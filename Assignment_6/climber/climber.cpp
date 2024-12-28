#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include "climber.h"
// add the include for climber.h
// --- Your code here
Coordinate offset_right{1, 0};
Coordinate offset_up{0, -1};
Coordinate offset_down{0, 1};
Coordinate offset_left{-1, 0};

int calculate_climbable(const std::pair<Coordinate, int> &current_position, const std::pair<Coordinate, int> &neighbor_position) {
    // Check if the neighbor is climbable
    if (neighbor_position.second > current_position.second && current_position.second + 2 >= neighbor_position.second) {
        return neighbor_position.second - current_position.second;
    }
    return -1;
}

std::vector<Coordinate> find_valid_neighbors(const Coordinate &current_position, const std::map<Coordinate, int> &height_map) {
    std::vector<Coordinate> valid_neighbors;
    try {
        height_map.at(current_position + offset_right);
        valid_neighbors.emplace_back(current_position + offset_right);
    } catch (const std::exception &e) {}

    try {
        height_map.at(current_position + offset_left);
        valid_neighbors.emplace_back(current_position + offset_left);
    } catch (const std::exception &e) {}

    try {
        height_map.at(current_position + offset_down);
        valid_neighbors.emplace_back(current_position + offset_down);
    } catch (const std::exception &e) {}

    try {
        height_map.at(current_position + offset_up);
        valid_neighbors.emplace_back(current_position + offset_up);
    } catch (const std::exception &e) {}

    return valid_neighbors;
}



// ---

bool operator==(Coordinate const &c1, Coordinate const &c2)
{
    return (c1.x == c2.x) && (c1.y == c2.y);
}

bool operator<(Coordinate const &c1, Coordinate const &c2)
{
    if (c1.x == c2.x)
    {
        return c1.y < c2.y;
    }
    return c1.x < c2.x;
}

Coordinate operator+(Coordinate const &c1, Coordinate const &c2)
{
    return {c1.x + c2.x, c1.y + c2.y};
}

void read_input(std::map<Coordinate, int> &m, Coordinate &start, std::string const &infilename)
{
    std::ifstream infile(infilename);
    if (!infile.good()) {
        std::cerr << "No such file " << infilename << '\n';
        throw std::runtime_error("No such file!");
    }

    infile >> start.x;
    infile >> start.y;
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // move to next line

    int x_idx = 0;
    int y_idx = 0;
    std::string line;
    while (std::getline(infile, line))
    {
        std::stringstream ssline(line);
        int val;
        while (ssline >> val)
        {
            m[{x_idx, y_idx}] = val;
            x_idx++;
        }
        x_idx = 0;
        y_idx++;
    }
}

void climber_algorithm(std::map<Coordinate, int> const &m, Coordinate const &start, std::string const& outfilename)
{
    std::ofstream outfile(outfilename);

    // These print statements are for your debugging convenience. The autograder will only look at the output file.
    std::cout << "height at start: " << m.at(start) << std::endl;

    Coordinate current_xy = start;
    int current_height = m.at(start);

    // 'offsets' should contain the changes in XY coordinates, in the order specified in the assigmnet doc.
    // Note that the Coordinate struct has operator+ defined on it, which lets you add two Coordiantes together
    std::vector<Coordinate> offsets{
        // --- Your code here

        find_valid_neighbors(current_xy, m)

        // ---
    };

    // We use a while true here, then break when we detect the climber has reached a local maximum
    while (true)
    {
        outfile << current_xy.x << " " << current_xy.y << " " << current_height << std::endl;
        // Consider creating variables to keep track of the next coordinate and the height at the next coordinate.
        // You should iterate over the valid neighbors and update these variables each time we find a new-best neighbor.
        // A valid neighor is one that is in-bounds, has height > the current height but <= the current height + 2.
        // If the current xy is the same as the next xy that means we've hit our local maxiumum and we should stop.
        
        // --- Your code here
        std::vector<Coordinate> climbable_neighbors;

        // Check all valid neighbors 
        for (int i = 0; i < offsets.size(); i++) {
            std::pair<Coordinate, int> neighbor_position_height = std::make_pair(offsets.at(i), m.at(offsets.at(i)));
            std::pair<Coordinate, int> current_position_height = std::make_pair(current_xy, current_height);
            int climbability_score = calculate_climbable(current_position_height, neighbor_position_height);

            if (climbability_score >= 0) {
                climbable_neighbors.emplace_back(neighbor_position_height.first);
            }
        }

        if (climbable_neighbors.empty()) {
            break;
        }

        std::sort(climbable_neighbors.begin(), climbable_neighbors.end(),
                [m](Coordinate &left_neighbor, Coordinate &right_neighbor) {
                    return m.at(left_neighbor) > m.at(right_neighbor);
                });

        current_xy = climbable_neighbors.at(0);
        current_height = m.at(climbable_neighbors.at(0));

        offsets = find_valid_neighbors(current_xy, m);
        // ---
    }
    std::cout << "location at end: " << current_xy.x << " " << current_xy.y << std::endl;
    std::cout << "height at end: " << current_height << std::endl;
}
