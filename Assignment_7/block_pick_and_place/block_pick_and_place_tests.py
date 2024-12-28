import numpy as np
import time
import unittest
from block_goal_publisher import read_goal, publish_transform
from block_pick_and_place_soln import PandaSimSolution
import transform_utils

panda = PandaSimSolution()

def test_ik(des_pose):
    q = panda.jacobian_ik(des_pose)
    act_pose, _ = panda.fk_and_jacobian(q)
    error = transform_utils.transform_error(des_pose, act_pose)
    return np.all(error < 1e-3)


class BlockPushingTestSuite(unittest.TestCase):

    def test_gripper_to_block_pose(self):
        block_pose = np.array([
                [0.7109135,  0.0000000,  0.7032794, 0.05],
                [0.0000000,  1.0000000,  0.0000000, 0.05],
                [-0.7032794, 0.0000000,  0.7109135, -0.5],
                [0.0000000,  0.000000,   0.0000000, 1.0]])
        gripper_pose_true = np.array([[ 0.7109135,  0.7032794,  0.,         0.05     ],
                                      [ 0.,         0.,        -1.,         0.065    ],
                                      [-0.7032794,  0.7109135,  0.,        -0.5      ],
                                      [ 0.,         0.,         0.,         1.       ]])
        gripper_pose = panda.block_pose_to_gripper_pose(block_pose)
        error = transform_utils.transform_error(gripper_pose, gripper_pose_true)
        close = np.all(error < 1e-3)
        self.assertTrue(close)

    def test_ik(self):
        des_pose = np.array([[1.0000000,  0.0000000,  0.0000000, 0.1],
                            [0.0000000, -0.0000000, -1.0000000, 0.065],
                            [0.0000000,  1.0000000, -0.0000000, -0.5],
                            [0.0000000,  0.0000000,  0.0000000, 1.0]])
        self.assertTrue(test_ik(des_pose))

    def test_lcm(self):
        panda.setup_thread()
        transform = read_goal('inputs1.txt')
        for _ in range(100):
            publish_transform(transform)
            time.sleep(0.01)
        true_transform = np.array([[0.7071068, 0., -0.7071068, -0.354],
                                    [0., 1., 0., 0.05],
                                    [0.7071068, 0., 0.7071068, -0.354],
                                    [0., 0., 0., 1. ]])
                                    
        self.assertIsNotNone(panda.goal_pose)
        transform_error = transform_utils.transform_error(true_transform, panda.goal_pose)
        self.assertTrue(np.all(transform_error < 1e-3))
        panda.exit()

if __name__ == "__main__":
    unittest.main()
