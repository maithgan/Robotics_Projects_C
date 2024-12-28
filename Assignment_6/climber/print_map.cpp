#include <string>
#include <iostream>
// --- Your code here
#include "climber.h"
// ---

void printmap(const std::map<Coordinate, int> &m)
{
    // print each entry in the map using the format "{x},{y} val: {value}" where {x} means print the value of the variable x
    // --- Your code here
    for (auto const& elem: m)
    {
        std::cout << elem.first.x << ","<< elem.first.y << " val: " << elem.second << std::endl;
    }
    // ---
}


int main(int argc, char *argv[])
{
    std::string infilename = "input.txt";
    std::string outfilename = "output.txt";
    
    // Allows the user to override the input and output filenames
    // by passing in arguments on the commandline.
    // You can ignore these, using the defaults is fine.
    if (argc == 2) { 
        infilename = argv[1];
    }
    if (argc == 3) { 
        infilename = argv[1];
        outfilename = argv[2];
    }
    std::map<Coordinate, int> m;
    Coordinate start;

    read_input(m, start, infilename);

    printmap(m);

    return EXIT_SUCCESS;
}