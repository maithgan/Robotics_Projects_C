#include "closest_pair.h"

#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <iostream>

int main() {
    std::ifstream input{"input.txt"};
    std::ofstream output{"output.txt"};

    // you can turn this to true to export the points and time your program
    auto timing = false;
    auto export_points = false;

    auto start = std::chrono::high_resolution_clock::now();

    int num;
    while (input.peek() != EOF) {
        int num, seed;
        input >> num >> seed;
        // generate the points
        std::vector<Point> points;
        std::random_device rd;
        std::mt19937 mt(rd());
        mt.seed(seed);
        std::uniform_real_distribution<double> dist(-1.0, 1.0);

        for (int i = 0; i < num; ++i) {
            points.push_back(Point{i, dist(mt), dist(mt)});
        }

        auto res = closest_pair(points);

        output << res.first.id << ' ' << res.first.x << ' ' << res.first.y
               << '\t' << res.second.id << ' ' << res.second.x << ' '
               << res.second.y << std::endl;

        if (export_points) {
            std::string name {"points"};
            name += std::to_string(seed);
            name += ".txt";
            std::ofstream ptout{name};

            for (const auto& pt : points) {
                ptout << pt.id << ' ' << pt.x << ' ' << pt.y << std::endl;
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    if (timing) {
        std::cout << duration.count() << std::endl;
    }
}