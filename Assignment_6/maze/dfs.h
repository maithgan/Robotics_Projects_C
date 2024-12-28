#pragma once
#include "maze.h"
// other includes
// --- Your code here
#include <stack>  
#include <vector> 
// ---

/** \brief Depth First Search solver */
class DFS : public TreeSearch {
   public:
    DFS();

    /** \brief Solve the planning problem. Override TreeSearch::solve() */
    bool solve();

    /** \brief add node to the tree */
    bool addNode(const NodePtr& node);

   private:
    // decide what data structure you need
    // --- Your code here
    std::stack<NodePtr> searchStack;
    std::vector<NodePtr> exploredNodes;
    // ---
};
using DFSPtr = std::shared_ptr<DFS>;