#include "maze.h"
#include "bfs.h"
#include "dfs.h"

/** \brief Return the valid next states from a given state */
std::vector<State> ProblemDefinition::validStates(const State &state) const
{
    // HINT: use isStateValid()
    // --- Your code here
    std::vector<std::vector<int>> offset;
    const std::vector<std::vector<int>> movementOffsets = allow_diagonal_
        ? std::vector<std::vector<int>>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}
        : std::vector<std::vector<int>>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    std::vector<State> validNextStates;
    for (const auto& offset : movementOffsets) {
        State nextState = state;
        nextState.x += offset[0];
        nextState.y += offset[1];

        // Add to valid states if the new state is valid
        if (isStateValid(nextState)) {
            validNextStates.push_back(nextState);
        }
    }

    return validNextStates;

    // ---
}

/** \brief Given a node, extract the path from it to the root */
void TreeSearch::extractPath(NodePtr node)
{
    // --- Your code here
    auto currentNode = node;
    while (currentNode->parent) {
        path_.push_back(currentNode->state);
        currentNode = currentNode->parent;
    }
    path_.push_back(currentNode->state);
    std::reverse(path_.begin(), path_.end());
    // ---
}

/** \brief Solve the planning problem. Override TreeSearch::solve() */
bool BFS::solve()
{
    // check start and goal are valid
    if (!pdef_.isStateValid(pdef_.startState()) ||
        !pdef_.isStateValid(pdef_.goalState()))
    {

        return false;
    }

    // if start and goal coincide, terminate
    auto start = pdef_.startState();
    if (pdef_.isGoal(start))
    {
        // std::cout << "Start and goal are equal\n";
        return true;
    }

    NodePtr solution;
    // --- Your code here
    auto initialNode = std::make_shared<Node>();  
    initialNode->state = start;
    initialNode->parent = NULL;
    addNode(initialNode);
    visited_.push_back(start);
    searchQueue.push(initialNode); 

    // search
    while (!searchQueue.empty()) {
        const auto &currentNode = searchQueue.front(); 
        searchQueue.pop();

        if (pdef_.isGoal(currentNode->state)) {
            std::cout << "found goal" << std::endl;
            solution = currentNode;
            break;
        }
        auto neighborStates = pdef_.validStates(currentNode->state); 
        for (const auto &neighbor : neighborStates) { 
            bool isNewState = std::find(visited_.begin(), visited_.end(), neighbor) == visited_.end(); 
            if (isNewState) {
                visited_.push_back(neighbor);
                auto newNode = std::make_shared<Node>();  
                newNode->state = neighbor;
                newNode->parent = currentNode;
                addNode(newNode);
                searchQueue.push(newNode); 
            }
        }
    }

    // ---
    // get the path from last node to root
    extractPath(solution);

    return true;
}

/** \brief add node to the tree */
bool BFS::addNode(const NodePtr &node)
{
    // --- Your code here
    exploredNodes.push_back(node); 
    return true;
    // ---
}


/** \brief Solve the planning problem. Override TreeSearch::solve() */
bool DFS::solve()
{
    // check start and goal are valid
    if (!pdef_.isStateValid(pdef_.startState()) ||
        !pdef_.isStateValid(pdef_.goalState()))
    {
        return false;
    }

    // if start and goal coincide, terminate
    auto start = pdef_.startState();
    if (pdef_.isGoal(start))
    {
        std::cout << "Start and goal are equal\n";
        return true;
    }

    NodePtr solution;
    // --- Your code here
    auto initialNode = std::make_shared<Node>();  
    initialNode->state = start;
    initialNode->parent = NULL;
    addNode(initialNode);
    visited_.push_back(start);
    searchStack.push(initialNode); 

    while (!searchStack.empty()) {
        const auto &currentNode = searchStack.top(); 
        searchStack.pop();

        if (pdef_.isGoal(currentNode->state)) {
            solution = currentNode;
            break;
        }

        std::cout << "check point: (" << currentNode->state.x << ", " << currentNode->state.y << ")" << std::endl;
        auto neighborStates = pdef_.validStates(currentNode->state); 
        for (const auto &neighbor : neighborStates) { 
            bool isNewState = std::find(visited_.begin(), visited_.end(), neighbor) == visited_.end(); 
            if (isNewState) {
                std::cout << "check candidate: (" << neighbor.x << ", " << neighbor.y << ")"
                          << " isNewState: " << isNewState << std::endl;
                visited_.push_back(neighbor);
                auto newNode = std::make_shared<Node>();  
                newNode->state = neighbor;
                newNode->parent = currentNode;
                addNode(newNode);
                searchStack.push(newNode); 
            }
        }
    }

    // ---
    // get the path from last node to root
    extractPath(solution);

    return true;
}

/** \brief add node to the tree */
bool DFS::addNode(const NodePtr &node)
{
    // --- Your code here
    exploredNodes.push_back(node); 
    return true;
    // ---
}
