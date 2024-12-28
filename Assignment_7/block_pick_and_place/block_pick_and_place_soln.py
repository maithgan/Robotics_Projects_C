import pybullet as p
import math
import numpy as np

from block_pick_and_place import PandaSim
import transform_utils

class PandaSimSolution(PandaSim):

    def jacobian_ik(self, goal_pose):
        """
        :param goal_pose: Desired end effector pose
        :return: Joint configurations

        """
        q = self.get_joint_state()
       
        for _ in range(1000):
            gripper_pose, J = self.fk_and_jacobian(q)
            dp = transform_utils.transform_error(goal_pose, gripper_pose)

            # --- Your code here
            dq = np.linalg.pinv(J).dot(dp.flatten())
            # ---

            # Limit the size of the change in joint config
            max_angle = np.pi / 8
            max_dq = np.max(np.abs(dq))
            if max_dq > max_angle:
                dq = dq * max_angle / max_dq
                      
            q = q + dq
            # wrap the joint config around
            q = np.where(q > np.pi, q - 2 * np.pi, q)
            q = np.where(q < -np.pi, q + 2 * np.pi, q)


        return q

    def block_pose_to_gripper_pose(self, block_pose):
        gripper_pose = None
        # --- Your code here
        R = np.array([[1, 0, 0],
                    [0, 0, -1],
                    [0, 1, 0]])

        T = np.array([0, 0.015, 0])
        
        H = np.eye(4)
        H[:3, :3] = R
        H[:3, 3] = T
        gripper_pose = block_pose @ H

        # --- 

        return gripper_pose

    def move_block_to_goal(self):
        """
            Moves block from current pose to a desired goal pose
        """

        # Get block pose
        block_pose = self.get_block_pose()

        # Get block pre-grasp pose
        gripper_pose = self.block_pose_to_gripper_pose(block_pose)

        # go to pre-grasp pose
        self.set_gripper_pose(gripper_pose)

        # Grasp the block
        self.grasp()

        # Get desired gripper pose to drop block at goal pose
        gripper_pose = self.block_pose_to_gripper_pose(self.goal_pose)

        # go to pose to drop block off
        self.set_gripper_pose(gripper_pose)

        # Drop the block off
        self.ungrasp()


if __name__ == "__main__":
    panda = PandaSimSolution()
    panda.setup_thread()
    panda.move_block_to_goal()
    panda.exit()
