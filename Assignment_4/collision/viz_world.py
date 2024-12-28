from ast import arg
import matplotlib.pyplot as plt
import argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--infile", default="robot_positions.txt")
    args = parser.parse_args()

    with open(args.infile, 'r') as f:
        lines = [l.strip("\n") for l in f.readlines()]

    for line in lines:
        fig = plt.figure()
        ax = plt.gca()

        x, y, radius = [float(value) for value in line.split(" ")]

        ax.set_xlim(0, 3)
        ax.set_ylim(0, 3)
        ax.set_aspect("equal")

        # world
        ax.add_patch(plt.Circle((2, 1), 0.5, color='m'))
        ax.add_patch(plt.Rectangle((0, 0), 1, 1, color='m'))
        ax.add_patch(plt.Rectangle((2, 1), 1, 1, color='m'))
        # robot
        ax.add_patch(plt.Circle((x, y), radius,
                     color='green', alpha=0.8))  # robot

        print("press q to close the window and visualize the next line in the file")
        plt.show()


if __name__ == '__main__':
    main()
