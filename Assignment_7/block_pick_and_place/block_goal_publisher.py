import lcm
import time
import numpy as np
from block_goal_msg import block_goal_t

def read_goal(fname):
    transformation = None
    # --- Your code here
    with open(fname, 'r') as file:
        lines = file.readlines()
        transformation = np.array([list(map(float, line.strip().split())) for line in lines])
        T = transformation[0]
        R = transformation[1:]
        transform = np.eye(4)
        transform[:3, :3] = R
        transform[:3, 3] = T
     # ---

    return transform

def publish_transform(transform):
    # --- Your code here
    lcm_obj = lcm.LCM()
    msg = block_goal_t()
    msg.transform = [[float(transform[i,j]) for j in range(4)] for i in range(4)]
    lcm_obj.publish("BLOCK_GOAL", msg.encode())

    # ---

if __name__ == "__main__":
 
    transform = read_goal('inputs1.txt')
    
    while True:
        publish_transform(transform)
        time.sleep(0.1)
