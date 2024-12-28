#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


using State = vector<int>; //this defines the type State to be a vector of ints
using Action = vector<int>; //this defines the type Action to be a vector of ints


//this function should print out a state to the given stream
//the ostream could be something like cout, or cerr
//or it could be a filestream
void printState(const State& state, ostream & stream){
    // --- Your code here
    stream << state[0] << " " << state[1] << endl;
    // ---
}


State dynamics(const State& state, const Action& action)
{
    State next_state(state.size());
    for (int j = 0; j < state.size();j++)
    {
        next_state[j] = state[j] + action[j];
    }
    return next_state;
}


int main () {
    
    ifstream policyfile("policy.txt");
    ofstream outputfile("output.txt");

    const int StateSize = 2;
    const int ActionSize = 2;    

    State key(StateSize);
    Action value(ActionSize);
    map<State,Action> policy;

    //read the policy file information into std::pair objects,
    //insert each std::pair into the map
    // --- Your code here
    while (policyfile >> key[0] >> key[1] >> value[0] >> value[1]) {
        policy[key] = value;
    }


    // ---

    //optional (but recommended) print out the policy to cout
    //to make sure that it was read in correctly
    // --- Your code here
    for (const auto& [state, action] : policy) {
        printState(state, cout); // Print the state
        cout << action[0] << " " << action[1] << endl; // Print the action 
    }


    // ---

    //read in rollout
    ifstream rolloutfile ("rollout.txt");
    State startState(StateSize); //this stores the start stae
    int numSteps; //this is the number of steps in the rollout
    rolloutfile>>startState[0];
    rolloutfile>>startState[1];
    rolloutfile>>numSteps;
    rolloutfile.close();

    //Now use the the policy map to do the rollout
    // --- Your code here
    State currentState = startState;
    printState(currentState, outputfile); // Print the start state

    for (int i = 0; i < numSteps; ++i) { //step by step iteration
        if (policy.find(currentState) == policy.end()) { //check if currentState is present in policy if not throw the below error message
            cerr << "State " << currentState[0] << " " << currentState[1] << " is not in policy, terminating." << endl;
            return 1;
        }

        Action action = policy[currentState];
        currentState = dynamics(currentState, action); //update current state to new value from dynamics
        printState(currentState, outputfile); // Print the next state
    }

    // ---

    outputfile.close();
    return 0;
}
