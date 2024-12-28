import lcm

import math
import time
import numpy as np
import threading

import pybullet as p
import pybullet_data as pd

from block_goal_msg import block_goal_t
from transform_utils import homogeneous_transform_to_pos_quat, pos_quat_to_homogeneous_transform

class PandaSim:

    def __init__(self, headless=False):
        if headless:
            p.connect(p.DIRECT)
        else:
            p.connect(p.GUI)

        # Change to Y up and change default camera pose
        p.configureDebugVisualizer(p.COV_ENABLE_Y_AXIS_UP, 1)
        p.resetDebugVisualizerCamera(cameraDistance=1, cameraYaw=60, cameraPitch=-30,
                                     cameraTargetPosition=np.array([0, 0.3, 0]))
        time.sleep(1)

        # Physics sim parameters
        self.dt = 1. / 100
        self.controller_steps = 100
        p.setTimeStep(self.dt)
        p.setGravity(0, -9.8, 0)

        # Load assets
        p.setAdditionalSearchPath(pd.getDataPath())
        flags = p.URDF_ENABLE_CACHED_GRAPHICS_SHAPES

        # load plane
        self.plane = p.loadURDF("plane.urdf", np.array([0.0, 0.0, 0.0]),
                                [-0.707107, 0.0, 0.0, 0.707107], flags)

        # Load Panda
        self.panda = p.loadURDF("assets/franka_panda/panda.urdf",
                                np.array([0, 0, 0]),
                                [-0.707107, 0.0, 0.0, 0.707107],
                                useFixedBase=True, flags=flags)

        # load object
        block_orn = p.getQuaternionFromEuler([0.0, 0.03 * math.pi / 2, 0.0])
        self.block = p.loadURDF("assets/block.urdf", np.array([0.0, 0.05, -0.5]),
                                block_orn, flags=flags,
                                globalScaling=1.5)

        # Creates constraint between gripper fingers so they move together when opening/closing
        c = p.createConstraint(self.panda,
                               9,
                               self.panda,
                               10,
                               jointType=p.JOINT_GEAR,
                               jointAxis=[1, 0, 0],
                               parentFramePosition=[0, 0, 0],
                               childFramePosition=[0, 0, 0])
        p.changeConstraint(c, gearRatio=-1, erp=0.1, maxForce=50)

        # Setup panda arm
        self.default_joint_pos = [0.98, 0.1, 0.1, -2.24, -0.30, 2.66, 2.32, 0.02, 0.02]
        index = 0
        for j in range(p.getNumJoints(self.panda)):
            p.changeDynamics(self.panda, j, linearDamping=10, angularDamping=10, jointLowerLimit=-99,
                             jointUpperLimit=99)
            info = p.getJointInfo(self.panda, j)
            jointType = info[2]
            if (jointType == p.JOINT_PRISMATIC):
                p.resetJointState(self.panda, j, self.default_joint_pos[index])
                index = index + 1
            if (jointType == p.JOINT_REVOLUTE):
                p.resetJointState(self.panda, j, self.default_joint_pos[index])
                index = index + 1

        # Goal position and orientation start as None
        self.goal_pose = None

        # Start up LCM and lock for threading
        self.lcm_obj = lcm.LCM()
        self.lock = threading.Lock()
        self.running = True

    def step(self):
        for _ in range(self.controller_steps):
            p.stepSimulation()
            time.sleep(self.dt)

    def get_block_pose(self):
        """
            Gets current block pose
        :return:
            pose: 4x4 homogeneous transform
        """

        pos, orn = p.getBasePositionAndOrientation(self.block)
        return pos_quat_to_homogeneous_transform(np.array(pos), np.array(orn))

    def set_gripper_pose(self, pose):
        """
            Sets gripper to desired pose

        :param
            pos: 4x4 homogeneous transform
        """
        joint_poses = self.jacobian_ik(pose)
        self.set_joint_state(joint_poses)

    def get_joint_state(self):
        """
            Gets current joint angles
        :return: state: 7 dimensional joint angles
        """
        state = np.zeros(7)
        for i in range(7):
            state[i] = p.getJointState(self.panda, i)[0]
        return state

    def set_joint_state(self, state):
        """
            Sets the joint state via the controller and simulates
        :param
            state: 7 dimensional joint angles
        """
        for i in range(7):
            p.setJointMotorControl2(self.panda, i, p.POSITION_CONTROL, state[i],
                                    force=5 * 240.)
        self.step()

    def get_gripper_pose(self):
        """
        :return:
            pose: 4x4 homogeneous transform
    
        """
        pandaEndEffectorIndex = 11
        pos, orn = p.getLinkState(self.panda, pandaEndEffectorIndex)[:2]
        return pos_quat_to_homogeneous_transform(np.array(pos), np.array(orn))

    def grasp(self):
        """
              Grasps an object. Lowers grippers, closes grippers, and then raises grippers
              Only successful if grippers are located in an appropriate pre-grasp pose.
        """
        # we lower the end effector slightly and then grasp
        gripper_pose = self.get_gripper_pose()
        gripper_pose[1, -1] -= 0.06
        self.set_gripper_pose(gripper_pose)

        for i in [9, 10]:
            p.setJointMotorControl2(self.panda, i,
                                    p.POSITION_CONTROL, 0.003,
                                    force=10)

        # now we rise up again to
        gripper_pose = self.get_gripper_pose()
        gripper_pose[1, -1] += 0.06
        self.set_gripper_pose(gripper_pose)

    def ungrasp(self):
        """
            Ungrasps an object. Lowers grippers, releases grippers, and then raises grippers
        """
        # we lower the end effector slightly and then grasp
        gripper_pose = self.get_gripper_pose()
        gripper_pose[1, -1] -= 0.03
        self.set_gripper_pose(gripper_pose)

        for i in [9, 10]:
            p.setJointMotorControl2(self.panda, i,
                                    p.POSITION_CONTROL, 0.04,
                                    force=10)

        # now we rise up again to
        gripper_pose = self.get_gripper_pose()
        gripper_pose[1, -1] += 0.03
        self.set_gripper_pose(gripper_pose)

    def fk_and_jacobian(self, joint_state):
        """
        Performs forward kinematics and computes the manipulator Jacobian
        :param joint_state: (np.array) 7 dimensional query joint state
        :return:
            gripper_pos (np.array): 3 dimensional gripper position
            gripper_orn (np.array): 4 dimensional gripper quaternion
            J (np.array): 6x7 dimensional manipulator Jacobian
        """

        pandaEndEffectorIndex = 11

        # We need to add zeros at the end for the grippers - these do not affect the jacobian so the values are
        # not important
        joint_state = np.concatenate((joint_state, np.zeros(2)), axis=0)

        # Save the previous joint state so that we can return to it
        current_joint_state = self.get_joint_state()

        # Set the joint state
        # We are not calling 'step' because we are not doing any physics simulation, we are pausing the simulation
        # and 'teleporting' the arm to a configuration in order to compute FK and the Jacobian
        for j in range(7):
            p.resetJointState(self.panda, j, joint_state[j], 0)

        # Calculate the jacobian
        linear_jacobian, angular_jacobian = p.calculateJacobian(self.panda, pandaEndEffectorIndex,
                                                                np.zeros(3).tolist(),
                                                                joint_state.tolist(),
                                                                np.zeros(9).tolist(),
                                                                np.zeros(9).tolist())
        # Get the gripper pose
        gripper_pose = self.get_gripper_pose()

        # Return the joints to their previous configuration
        for j in range(7):
            p.resetJointState(self.panda, j, current_joint_state[j], 0)

        return gripper_pose, np.vstack([np.array(linear_jacobian), np.array(angular_jacobian)])[:, :7]

    def block_goal_handler(self, channel, data):
        self.lock.acquire()
        msg = block_goal_t.decode(data)
        self.goal_pose = np.array(msg.transform)
        self.lock.release()
        
    def block_goal_thread_fn(self):
        while self.running:
            self.lcm_obj.handle()
            time.sleep(0.01)

    def setup_thread(self):
        self.running = True
        self.lcm_obj.subscribe('BLOCK_GOAL', self.block_goal_handler)
        worker = threading.Thread(target=self.block_goal_thread_fn)
        worker.start()

    def exit(self):
        self.running = False

