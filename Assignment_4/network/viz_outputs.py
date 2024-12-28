import matplotlib.pyplot as plt
import numpy as np

class_names = {
}


def plot_example(ax, true_i, image_flat, class_probabilities):
    img = image_flat.reshape(28, 28)
    max_p = np.max(class_probabilities)
    max_i = np.argmax(class_probabilities)
    ax.imshow(img)
    ax.set_title(
        f"True Class: {true_i}\nPredicted Class: {max_i}({max_p} confidence)")


def main():
    input_filenames = [
        "network/data1.csv",
        "network/data2.csv",
        "network/data3.csv",
        "network/data4.csv",
    ]

    def _parse_data_file(filename):
        with open(filename, 'r') as f:
            lines = [line.strip("\n") for line in f.readlines()]
            true_class = int(lines[-1])
        img = np.loadtxt(filename)[2:-1]  # skip first 2 and last 1
        return true_class, img
    inputs = [_parse_data_file(filename) for filename in input_filenames]
    outputs = np.loadtxt("network/output.csv", delimiter=',')
    fig, ax = plt.subplots(2, 2)
    plot_example(ax[0, 0], *inputs[0], outputs[0])
    plot_example(ax[0, 1], *inputs[1], outputs[1])
    plot_example(ax[1, 0], *inputs[2], outputs[2])
    plot_example(ax[1, 1], *inputs[3], outputs[3])
    plt.show()


if __name__ == '__main__':
    main()
