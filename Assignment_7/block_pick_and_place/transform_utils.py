import numpy as np
from scipy.spatial.transform import Rotation

def quaternion_multiply(quaternion1, quaternion0):
    x0, y0, z0, w0 = quaternion0
    x1, y1, z1, w1 = quaternion1
    return np.array([
        x1 * w0 + y1 * z0 - z1 * y0 + w1 * x0,
        -x1 * z0 + y1 * w0 + z1 * x0 + w1 * y0,
        x1 * y0 - y1 * x0 + z1 * w0 + w1 * z0,
        -x1 * x0 - y1 * y0 - z1 * z0 + w1 * w0
    ])


def quaternion_conjugate(quaternion):
    q = np.array(quaternion, copy=True)
    np.negative(q[:-1], q[:-1])
    return q


def orientation_error(desired, current):
    cc = quaternion_conjugate(current)
    q_r = quaternion_multiply(desired, cc)
    return q_r[:-1] * np.sign(q_r[-1])


def quaternion_to_matrix(quaternion):
    return Rotation.from_quat(quaternion).as_matrix()


def matrix_to_quaternion(matrix):
    return Rotation.from_matrix(matrix).as_quat()

def homogeneous_transform_to_pos_quat(H):
    """
        Converts homogeneous transform (rotation first) to 
        translation + quaternion (translation first)
    """

    R = H[:3, :3]
    p = H[:3, -1]
    
    quat = matrix_to_quaternion(R)
    return p, quat

def pos_quat_to_homogeneous_transform(pos, quat):
    """
        Converts translation + quaternion (translation first) to 
        homogenous transform (rotation first)
    """
    R = quaternion_to_matrix(quat)
    H = np.concatenate((R, pos.reshape(3, 1)), axis=1)
    H = np.concatenate((H, np.array([0., 0., 0., 1.]).reshape(1, 4)), axis=0)
    return H

def transform_error(desired_pose, current_pose):
    dpos, dquat = homogeneous_transform_to_pos_quat(desired_pose)
    pos, quat = homogeneous_transform_to_pos_quat(current_pose)
    orn_error = orientation_error(dquat, quat)
    pos_error = dpos - pos
    R = current_pose[:3, :3]
    error = np.concatenate((R @ pos_error, R @ orn_error), axis=0)
    return error
