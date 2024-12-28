import argparse
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--infile", type=str, default='pointcloud1.txt')
    parser.add_argument("-a", type=float, default=1.0)
    parser.add_argument("-b", type=float, default=0.1)
    parser.add_argument("-c", type=float, default=-1.0)
    parser.add_argument("-d", type=float, default=0.0)

    args = parser.parse_args()

    # show the plane given the sudent's answer
    normal = np.array([args.a, args.b, args.c])
    v1 = np.cross([1, 0, 0], normal)
    v2 = np.cross(v1, normal)
    student_points = np.array([v1 + v2, v1 - v2, -v1 + v2, -v1-v2])
    student_points[:, 2] += -args.d/args.c

    # show the plane points from input.txt
    with open(args.infile, 'r') as f:
        lines = f.readlines()
    input_xx = []
    input_yy = []
    input_zz = []
    for l in lines[1:]:
        words = l.strip("\n").split(" ")
        input_xx.append(float(words[0]))
        input_yy.append(float(words[1]))
        input_zz.append(float(words[2]))

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(input_xx, input_yy, input_zz)
    verts = [
        [student_points[0], student_points[1], student_points[2]], # first triangle
        [student_points[1], student_points[2], student_points[3]], # second triangle
    ]
    bbox = Poly3DCollection(verts, alpha=.6)
    bbox.set_facecolor('black')
    ax.add_collection3d(bbox)
    plt.show()


if __name__ == '__main__':
    main()
