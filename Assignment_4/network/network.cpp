#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <eigen3/Eigen/Eigen>

Eigen::MatrixXd csv2mat(std::ifstream &ifs)
{
    if (!ifs.good())
    {
        throw std::runtime_error("failed to open file!");
    }

    int rows, cols;
    ifs >> rows;
    ifs >> cols;
    Eigen::MatrixXd mat(rows, cols);

    int row = 0;
    int col = 0;
    while (ifs.peek() != ifs.eof())
    {
        double x;
        ifs >> x;
        mat(row, col) = x;
        ++col;
        if (col == cols)
        {
            col = 0;
            ++row;
        }
        if (row == rows)
        {
            break;
        }
    }
    return mat;
}

class Layer
{
public:
    Layer() {};

    virtual Eigen::MatrixXd forward(Eigen::MatrixXd const &x) const = 0;
};

class Linear : public Layer
{
public:
    Linear(std::string const &A_filename, std::string const &b_filename)
    {
        std::ifstream A_file(A_filename);
        std::ifstream b_file(b_filename);
        A = csv2mat(A_file);
        b = csv2mat(b_file);
    }

    Eigen::MatrixXd forward(Eigen::MatrixXd const &x) const override
    {
        // --- Your code here
        return (A*x+b); 
        // ---
    };

private:
    Eigen::MatrixXd A;
    Eigen::MatrixXd b;
};

// Create and implement the ReLU and Softmax classes here
// --- Your code here
class ReLU: public Layer {

public:
    ReLU() {};  
    Eigen::MatrixXd forward(Eigen::MatrixXd const &x) const override
    {
    return x.cwiseMax(0);
    };

private:
    Eigen::MatrixXd x;
};

class Softmax: public Layer{
public:
    Softmax() {};
    Eigen::MatrixXd forward(Eigen::MatrixXd const &x) const override
    {
    Eigen::MatrixXd exp = x.array().exp();
    exp = exp / (exp.sum());
    return exp;
    };

private:
    Eigen::MatrixXd x;
};

// ---

int main(int argc, char* argv[])
{
    const Eigen::IOFormat vec_csv_format(3, Eigen::DontAlignCols, ", ", ", ");
    std::ofstream ofs("output.csv");

    // load in the weights, biases, and the data from files
    std::vector<std::string> data_filenames{"data1.csv", "data2.csv", "data3.csv", "data4.csv"};
    if (argc >= 2) {
        data_filenames.clear();
        for (int i{1}; i < argc; ++i) {
            data_filenames.push_back(argv[i]);
        }
    }

    Linear l1("A1.csv", "b1.csv");
    ReLU r;
    Linear l2("A2.csv", "b2.csv");
    Softmax s;

    for (std::string const &data_filename : data_filenames)
    {
        std::cout << "Evaluating " << data_filename << '\n';
        std::ifstream ifs{data_filename};
        Eigen::MatrixXd X = csv2mat(ifs);

        // now call your layers
        // --- Your code here
        auto probabilities = s.forward(l2.forward(r.forward(l1.forward(X))));
        // ---
        ofs << probabilities.format(vec_csv_format) << std::endl;
    }
}