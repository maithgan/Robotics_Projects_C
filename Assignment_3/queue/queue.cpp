#include "robots.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;


//define and implement a function here called compareBotsByName
//it should be of the proper form to be used as the comparison
//function in std::sort. You will use compareBotsByName and std::sort
//to sort a std::vector<Robot>
// --- Your code here
bool compareBotsByName(const Robot &a, const Robot &b){
  return a.getName()<b.getName();
}


// ---


int main() {
  ifstream botfile("bots.txt");
  ifstream jobfile("jobs.txt");
  ofstream outfile("output.txt");
  
  queue<Robot> bot_queue;

  //read the robots from botfile in to bot_queue
  //make sure you've looked at the definition 
  //of the Robot class in robots.h first
  // --- Your code here
  string bot_name, bot_type;
  while (botfile >> bot_name >> bot_type) {
    bot_queue.push(Robot(bot_name, bot_type));
  }


  // ---

  //read in each job id and job in jobfile
  //and assign them to the robots as described in the problem
  // --- Your code here
  while (jobfile.peek() != std::ifstream::traits_type::eof()) {
    int id;
    string job;
    jobfile >> id;
    jobfile >> job;
    if (job.size() == 0) {
      break;
    }
    
    bool assigned = false;
    while (!assigned){
      auto bot = bot_queue.front();
      auto type = bot.getType();
      auto jobtypes = TYPES2JOBS.at(type);
      for (const auto& jobtype: jobtypes) {
        if (job == jobtype) {
          bot.assignJob(id);
          assigned = true;
          break;
        }
      }
      bot_queue.pop();
      bot_queue.push(bot);

    }
  }
  // ---

  
  vector<Robot> bots;

  //now that the jobs are assigned, get the robots from the queue
  //and put them in the bots vector
  //then sort that vector alphabetically using std::sort
  //you will need to use the compareBotsByName function above
  // --- Your code here
  
  // Move all robots from queue to vector
  while (!bot_queue.empty()) {
      bots.push_back(bot_queue.front());
      bot_queue.pop();
  }

  // Sort robots by name using the compareBotsByName function
  sort(bots.begin(), bots.end(), compareBotsByName);


  // ---

  // print the bots to outfile and cout
  for (int i = 0; i < bots.size(); i++) {
    cout << bots[i].print(false);
    outfile << bots[i].print(false);
  }

  return 0;
}
