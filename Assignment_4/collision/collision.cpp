#include <eigen3/Eigen/Eigen>
#include <fstream>
#include <iostream>
#include <vector>

#include "collision.h"


int main(int argc, char* argv[])
{
    // when submitting to autograder, make sure you use exactly the 'main' function we gave you!
    Disc robot{0, 0, 1}; // dummy values initially
    Rectangle rect1{0, 0, 1, 1};
    Rectangle rect2{2, 1, 3, 2};
    Disc disc1{2, 1, 0.5};

    std::vector<Obstacle const *> world{&rect1, &rect2, &disc1};

    // by defult, use the file robot_positions.txt, but if you supply a command line argument, use that instead
    std::string input_filename{"robot_positions.txt"};
    if (argc >= 2) {
        input_filename = argv[1];
    }
    std::ifstream ifs(input_filename);

    if (!ifs.good()) {
        std::cerr << "Failed to open file " << input_filename << std::endl;
        return 0;
    }

    while (ifs.good()) {
        double robot_x, robot_y;
        ifs >> robot_x;
        ifs >> robot_y;
        robot.pos_ << robot_x, robot_y; // set the position of the robot
        ifs >> robot.radius_; // set the robot radius
        std::cout << check_collisions(world, robot) << std::endl;
    }

    return 0;
}