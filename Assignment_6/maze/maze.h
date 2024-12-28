#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

/** \brief State representing a 2D cell with indexes x, y */
struct State {
    int x;
    int y;

    State();
    State(int x, int y);
    ~State();

    void print() const;
};
using StatePtr = std::shared_ptr<State>;

/** \brief Node representing a state in a path */
class Node {
   public:
    Node();
    virtual ~Node();

    /** \brief The state */
    State state;

    /** \brief Pointer to the state's parent */
    std::shared_ptr<Node> parent;

    /** \brief The cost of the path from root to state */
    double cost_to_come;

    /** \brief The estimated cost of the path from root to goal through state
     *  (sum of cost to come and an estimate of the cost to go) */
    double estimated_cost;

    void print() const;
};
using NodePtr = std::shared_ptr<Node>;

/** \brief Define operator == between State objects.
 * This is necessary to check is two states correspond to the same location */
bool operator==(const State& s1, const State& s2);

/** \brief Definition of a planning problem */
class ProblemDefinition {
   private:
    /** \brief Matrix representing a 2D maze
     *  Column index represent x axis from left to right
     *  Row index represents y axis from bottom to top */
    std::vector<std::vector<bool>> maze_;

    /** \brief Row and column size of maze_ */
    std::vector<int> maze_size_;

    /** \brief The start state */
    State start_;

    /** \brief The goal state */
    State goal_;

   public:
    /** \brief Constructor */
    ProblemDefinition();

    /** \brief Destructor */
    virtual ~ProblemDefinition();

    State goalState() const;

    State startState() const;

    /** \brief Check if a state satisfies the goal */
    bool isGoal(const State& state) const;

    /** \brief If true, allows for diagonal moves between cells */
    bool allow_diagonal_{false};

    /** \brief Read the maze from txt file */
    void readMaze(std::string filename);

    std::vector<std::vector<bool>> getMaze() const;

    /** \brief Print maze_ on std::cout
     *  If a path is given, it is printed as '*'
     * Default: empty path
     */
    void printMaze(const std::vector<State>& path = std::vector<State>()) const;

    /** \brief Return the valid next states from a given state */
    std::vector<State> validStates(const State& state) const;

    /** \brief check that a state is valid (within the maze and not in
     * collision) */
    bool isStateValid(const State& state) const;
};
using ProblemDefinitionPtr = std::shared_ptr<ProblemDefinition>;

/** \brief An abstract class from which other solver should inherit */
class TreeSearch {
   public:
    TreeSearch();
    TreeSearch(std::string name);

    virtual ~TreeSearch();

    void setProblemDefinition(const ProblemDefinition& pdef);

    std::vector<State> getSolution() const;

    void printSolution() const;

    /** \brief Write the solution path to txt file */
    void saveSolution(std::string filename) const;

    /** \brief Solve the planning problem (pure virtual method) */
    virtual bool solve() = 0;

    /** \brief The planning problem */
    ProblemDefinition pdef_;  // NOTE this could be considered bad practice. Instead of public this should be protected

    /** \brief Given a node, extract the path from it to the root; save the path
     * to path_ */
    void extractPath(NodePtr node);


    bool isVisited(const NodePtr &node) const;

    /** \brief Add a node to the solver
     *  Virtual method. Should be redefined in child solvers */
    virtual bool addNode(const NodePtr& node) = 0;

    /** \brief List of visited nodes */
    std::vector<State> visited_;

   private:
    /** \brief The node of the solver */
    std::string name_{"TreeSearch"};

    /** \brief The path */
    std::vector<State> path_;
};
using TreeSearchPtr = std::shared_ptr<TreeSearch>;
