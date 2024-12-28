import argparse
import matplotlib.pyplot as plt
import numpy as np
import re


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("points", type=str)
    parser.add_argument("--output", type=str, default=None)
    parser.add_argument("--index", type=int, default=0)

    args = parser.parse_args()

    with open(args.points, 'r') as f:
        lines = f.readlines()
        pts = [[float(pt) for pt in line.strip().split()] for line in lines]
        pts = np.array(pts)

    fig = plt.figure()
    ax = fig.add_subplot(111)

    ax.set_xlim([-1, 1])
    ax.set_ylim([-1, 1])
    ax.scatter(pts[:, 1], pts[:, 2], alpha=0.1)

    if args.output:
        with open(args.output, 'r') as f:
            lines = f.readlines()
            line = lines[args.index].strip().split()
            pts_pair = pts[[int(line[0]), int(line[3])]]
            ax.scatter(pts_pair[:, 1], pts_pair[:, 2])

    plt.show()


if __name__ == '__main__':
    main()
