from matplotlib import pyplot as plt
from matplotlib.patches import Rectangle
import numpy as np
import argparse


def draw_patch(ax, xy, rgba):
    if ax is None:
        return
    rect = Rectangle((xy[1], xy[0]), 1, 1, edgecolor=None, facecolor=rgba)
    ax.add_patch(rect)


def draw_state(ax, xy, previous=None, color='b', text=None):
    if ax is None:
        return
    ax.scatter(xy[1] + 0.5, xy[0] + 0.5, color=color)
    if previous is not None:
        ax.arrow(previous[1] + 0.5, previous[0] + 0.5, xy[1] - previous[1], xy[0] - previous[0], fc=color, ec=color)
    if text is not None:
        ax.text(xy[1] + 0.25, xy[0] + 0.25, text)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=str)
    parser.add_argument("output", type=str)
    parser.add_argument("--eval", action="store_true")

    args = parser.parse_args()

    with open(args.input, 'r') as f:
        line = f.readline()
        nums = [int(v) for v in line.strip().split()]
        start_state = np.array(nums[:2])
        allow_diagonal = bool(nums[2])

        line = f.readline()
        nums = [int(v) for v in line.strip().split()]
        goal_state = np.array(nums)

        maze = f.readlines()
        maze = [line.strip().split() for line in maze]

    rows = len(maze)
    cols = len(maze[0])

    fig = None
    ax = None
    if not args.eval:
        fig, ax = plt.subplots(figsize=(rows, cols))
        ax.set_aspect('equal')
        ax.set_xlim([0, cols])
        ax.set_ylim([0, rows])
        ax.invert_yaxis()
        plt.show(block=False)

    draw_patch(ax, start_state, (0, 0, 1, 0.5))
    draw_patch(ax, goal_state, (0, 1, 0, 0.5))

    obstacles = set()
    for i in range(rows):
        for j in range(cols):
            if maze[i][j] == 'X':
                obstacles.add((i, j))
                draw_patch(ax, (i, j), (0, 0, 0, 1.0))

    colors = "bry"
    ci = 0

    draw_state(ax, start_state, color=colors[ci], text='start')
    draw_state(ax, goal_state, color=colors[ci], text='goal')
    with open(args.output, 'r') as f:
        lines = f.readlines()
        i = 0
        state = start_state
        prev_state = None
        while i < len(lines):
            method = lines[i].strip()
            if fig is not None:
                fig.suptitle(method)
            i += 1
            while lines[i].strip() != '':
                draw_state(ax, state, prev_state, color=colors[ci])
                xy = tuple(int(v) for v in lines[i].strip().split())
                prev_state = state
                state = xy
                if state in obstacles:
                    print(f"STATE {state} in an obstacle!!!")
                    # exit(1)
                if not args.eval:
                    plt.pause(0.1)
                i += 1
            i += 1
            draw_state(ax, state, prev_state, color=colors[ci])

            if args.eval:
                if state[0] == goal_state[0] and state[1] == goal_state[1]:
                    print(f"{method} ARRIVED")
                else:
                    print(f"{method} FAILED")
            else:
                plt.pause(2)
            ci += 1
            state = start_state
            prev_state = None

    if not args.eval:
        plt.savefig('end.png')
