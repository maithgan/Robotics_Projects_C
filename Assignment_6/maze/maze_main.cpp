#include "maze.h"
#include "bfs.h"
#include "dfs.h"

int main(int argc, char *argv[]) {
    std::string inputfile = "input.txt";
    std::string outfile = "output.txt";

    if (argc == 2) {
        inputfile = argv[1];
    }

    // read maze from inputfile
    ProblemDefinition pdef;
    pdef.readMaze(inputfile);

    // delete content of outfile
    std::ofstream ofs;
    ofs.open(outfile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    // solve with BFS
    std::cout << "\nBFS\n";
    BFS alg1;
    alg1.setProblemDefinition(pdef);
    bool solved = alg1.solve();

    if (solved) {
        alg1.printSolution();
        alg1.saveSolution(outfile);
    } else {
        std::cout << "BFS could not find a solution.\n";
    }

    // solve with DFS
    std::cout << "\nDFS\n";
    DFS alg2;
    alg2.setProblemDefinition(pdef);
    solved = alg2.solve();

    if (solved) {
        alg2.printSolution();
        alg2.saveSolution(outfile);
    } else {
        std::cout << "DFS could not find a solution.\n";
    }

    return 0;
}
