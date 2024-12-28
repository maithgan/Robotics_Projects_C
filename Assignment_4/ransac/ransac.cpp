#include <iostream>
#include <chrono>
#include <random>
#include <eigen3/Eigen/Eigen>
#include <fstream>
#include <iomanip>

#include "ransac.h"

int main(int argc, char* argv[])
{
    // by defult, use the file pointcloud.txt, but if you supply a command line argument, use that instead
    std::string input_filename{"pointcloud1.txt"};
    if (argc >= 2) {
        input_filename = argv[1];
    }
    std::ifstream ifs(input_filename);

    if (!ifs.good()) {
        std::cerr << "Failed to open file " << input_filename << std::endl;
        return 0;
    }


    int num_points;
    ifs >> num_points;
    Eigen::MatrixXd points(num_points, 3);

    for (int i{0}; i < num_points; ++i)
    {
        double x, y, z;
        ifs >> x;
        ifs >> y;
        ifs >> z;
        points(i, 0) = x;
        points(i, 1) = y;
        points(i, 2) = z;
    }

    // now run RANSAC

    AnalyticFitter af(num_points);
    LeastSquaresFitter lf(num_points, 10);

    std::ofstream ofs{"planes.txt"};

    {
        Plane const af_plane = ransac(af, points);
        ofs << af_plane << "\n";
        pretty_print(input_filename, af_plane);
    }

    {
        Plane const lf_plane = ransac(lf, points);
        ofs << lf_plane << "\n";
        // these print statements are useful for passing into viz_plane.py
        pretty_print(input_filename, lf_plane);
    }
}