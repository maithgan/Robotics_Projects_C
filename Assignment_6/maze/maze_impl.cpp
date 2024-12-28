#include "maze.h"
#include "bfs.h"
#include "dfs.h"

State::State() {}
State::State(int x, int y) : x(x), y(y) {}
State::~State() {}
void State::print() const { std::cout << x << ", " << y << std::endl; }

Node::Node() { parent = nullptr; }
Node::~Node() {}
void Node::print() const
{
    std::cout << state.x << ", " << state.y << ", " << cost_to_come << ", "
              << estimated_cost << std::endl;
}

bool operator==(const State &s1, const State &s2)
{
    return (s1.x == s2.x && s1.y == s2.y);
}

/** \brief Constructor */
ProblemDefinition::ProblemDefinition()
{
    maze_.clear();
    maze_size_.resize(2);
}

/** \brief Destructor */
ProblemDefinition::~ProblemDefinition()
{
    maze_.clear();
    maze_size_.clear();
}

State ProblemDefinition::goalState() const { return goal_; }

State ProblemDefinition::startState() const { return start_; }

bool ProblemDefinition::isGoal(const State &state) const
{
    return state == goal_;
}

void ProblemDefinition::readMaze(std::string filename)
{
    std::ifstream fin;
    fin.open(filename); // open file

    std::string line;
    std::vector<std::vector<bool>> mat;

    int row_id = 0;
    while (std::getline(fin, line)) // read file line-by-line
    {
        std::istringstream iss(line); // process each line separately

        // the first two lines are start and goal respectively
        if (row_id < 2)
        {
            int x;
            std::vector<int> row;
            while (iss >> x)
            {
                row.push_back(x);
            }

            // use first line as start state
            if (row_id == 0)
            {
                start_.x = row[0];
                start_.y = row[1];
                allow_diagonal_ = row[2] == 1;
            }
            // use second line as goal state
            else if (row_id == 1)
            {
                goal_.x = row[0];
                goal_.y = row[1];
            }
        }
        // the actual maze
        else
        {
            char x;
            std::vector<bool> row;

            // evaluate each element of the row
            while (iss >> x)
            {
                // char 'X' is considered an obstacle
                if (x == 'X')
                {
                    row.push_back(true);
                }
                // char '0' is considered free space
                else if (x == '0')
                {
                    row.push_back(false);
                }
            }

            // push all element in a matrix. Note: this is not the actual
            // maze
            mat.push_back(row);
        }
        row_id++;
    }

    start_.print();
    goal_.print();

    if (mat.size() == 0)
    {
        std::cout << "\nERROR: problem initialization failed.\n"
                  << " Possible problems are:\n"
                  << "1. The input txt file is corrupt or empty\n"
                  << "2. You run the program from a folder different from "
                     "the input file path\n\n";
        return;
    }

    // Define size of the maze. Notice that rows(maze_) = cols(mat) and vice
    // versa
    maze_.resize(mat.size());
    for (int i = 0; i < maze_.size(); ++i)
    {
        maze_[i].resize(mat[0].size());
    }

    // Revert and tranpose the matrix to adjust axis orientation
    // Needed because the maze should have x axis from bottom to top and y
    // axis from left to right
    // std::reverse(mat.begin(), mat.end());  // y axis is from bottom to top
    for (int i = 0; i < mat.size(); ++i)
    {
        for (int j = 0; j < mat[0].size(); ++j)
        {
            maze_[i][j] = mat[i][j];
        }
    }

    maze_size_[0] = maze_.size();    // number of rows
    maze_size_[1] = maze_[0].size(); // number of columns

    /* Check start and goal consistency with collision and maze bounds */

    if (!isStateValid(start_))
    {
        std::cout << "\nERROR: start state not valid!\n"
                  << std::endl;
    }

    if (!isStateValid(goal_))
    {
        std::cout << "\nERROR: goal state not valid!\n"
                  << std::endl;
    }
}

std::vector<std::vector<bool>> ProblemDefinition::getMaze() const
{
    return maze_;
}

void ProblemDefinition::printMaze(const std::vector<State> &path) const
{
    std::cout << "Number of rows: " << maze_size_[0] << std::endl;
    std::cout << "Number of columns: " << maze_size_[1] << std::endl;

    std::cout << "Start: " << start_.x << ", " << start_.y << std::endl;
    std::cout << "Goal: " << goal_.x << ", " << goal_.y << std::endl;

    std::vector<std::vector<char>> mat(maze_.size());
    for (int i = 0; i < maze_.size(); i++)
    {
        mat[i].resize(maze_[i].size());
        for (int j = 0; j < maze_[i].size(); j++)
        {
            if (maze_[i][j])
            {
                mat[i][j] = 'X';
            }
            else
            {
                mat[i][j] = '0';
            }
        }
    }

    // if a path is given, set '*' to the cells traversed by the path
    for (const auto &state : path)
    {
        mat[state.x][state.y] = '*';
    }

    for (const auto &row : mat)
    {
        for (const auto &x : row)
        {
            std::cout << x << '\t';
        }
        std::cout << '\n';
    }
}

bool ProblemDefinition::isStateValid(const State &state) const
{
    if (maze_.empty())
    {
        std::cout << "\nERROR: maze not initialized. Cannot check state "
                     "validity.\n";
        return false;
    }

    // out of bounds
    if (state.x < 0 || state.x > maze_size_[0] - 1 || state.y < 0 || state.y > maze_size_[1] - 1)
    {
        return false;
    }

    if (maze_[state.x][state.y] == true)
    { // in collision
        return false;
    }

    return true; // free space
}

TreeSearch::TreeSearch() {}

/** \brief Constructor */
TreeSearch::TreeSearch(std::string name) : name_(name) {}

TreeSearch::~TreeSearch() {}

void TreeSearch::setProblemDefinition(const ProblemDefinition &pdef)
{
    pdef_ = pdef;
}

std::vector<State> TreeSearch::getSolution() const { return path_; }

void TreeSearch::printSolution() const
{
    std::cout << "Path has " << path_.size() << " states:\n";
    for (const auto &s : path_)
    {
        s.print();
    }
}

void TreeSearch::saveSolution(std::string filename) const
{
    // open file
    std::ofstream out_file;
    out_file.open(filename, std::ios_base::app);

    // write solver name
    out_file << name_ << "\n";

    // write path states as rows
    for (const auto &s : path_)
    {
        out_file << s.x << " " << s.y << "\n";
    }
    out_file << "\n";

    // close file
    out_file.close();
    std::cout << "Saved in file output.txt \n";
}

bool TreeSearch::isVisited(const NodePtr &node) const
{
    return std::find(visited_.cbegin(), visited_.cend(), node->state) != visited_.end();
}

BFS::BFS() : TreeSearch::TreeSearch("BFS") {}
DFS::DFS() : TreeSearch::TreeSearch("DFS") {}