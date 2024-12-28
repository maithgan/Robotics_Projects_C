#include <fstream>
#include <iomanip>
#include <iostream>
#include <eigen3/Eigen/Eigen>

Eigen::Matrix3d transform_mat(double q, double l)
{
    // --- Your code here
    Eigen::Matrix3d Homogeneous = Eigen::Matrix3d::Zero();
    Eigen::Matrix2d Rotation;
    Rotation << cos(q), -sin(q), sin(q), cos(q);
    
    Homogeneous.block(0, 0, 2, 2) = Rotation;
    Homogeneous(0,2) = l;
    Homogeneous(2,2) = 1;
    return Homogeneous;
    // ---
}

int main(int argc, char* argv[])
{
    // by defult, use the file joint_angles.txt, but if you supply a command line argument, use that instead
    std::string input_filename{"joint_angles.txt"};
    if (argc >= 2) {
        input_filename = argv[1];
    }
    std::ifstream ifs(input_filename);

    if (!ifs.good()) {
        std::cerr << "Failed to open file " << input_filename << std::endl;
        return 0;
    }

    double l1{1.0};
    double l2{0.5};
    double l3{0.25};

    while (ifs.peek() != -1)
    {
        double q1, q2, q3;
        ifs >> q1 >> q2 >> q3;

        // --- Your code here

        Eigen::Matrix3d Link_01 = transform_mat(q1, 0); //base to link1
        Eigen::Matrix3d Link_12 = transform_mat(q2, l1); //link1 to 2
        Eigen::Matrix3d Link_23 = transform_mat(q3, l2); // link2 to 3
        Eigen::Matrix3d Link_3e = transform_mat(0, l3); // link3 to end-effector


        Eigen::Matrix3d Homogeneous_final = Link_01 * Link_12 * Link_23 * Link_3e;
        std::cout << std::setprecision(3) << Homogeneous_final.block<2,1>(0,2) << std::endl;

        return 0;
        // ---
    }
}