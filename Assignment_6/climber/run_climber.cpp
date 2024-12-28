#include <string>
// --- Your code here
#include "climber.h"
// ---

int main(int argc, char *argv[])
{
    // NOTE: these filenames are relative to the current working directory.
    // if you get a "file not found" error, then you (or vscode) are not running the executable from
    // the right directory. Check that your "settings.json" file in the ".vscode" folder contains:
    //   "cmake.debugConfig": {
    //    "cwd": "${workspaceFolder}"
    //   }
    std::string infilename = "input.txt";
    std::string outfilename = "output.txt";
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

    climber_algorithm(m, start, outfilename);

    return EXIT_SUCCESS;
}