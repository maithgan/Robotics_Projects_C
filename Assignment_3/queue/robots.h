#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <vector>

class Robot {
public:
  Robot(const std::string &n, const std::string &t) {
    name = n;
    type = t;
  };

  std::string print(bool bDebugPrint) const {
    std::stringstream out;
    if (bDebugPrint) {
      out << "Name: " << name << " Type: " << type << std::endl;
      out << "Assigned Job IDs: ";
    } else
      out << name << " ";

    for (int i = 0; i < assigned_jobs.size(); i++)
      out << assigned_jobs[i] << " ";
    out << std::endl;
    return out.str();
  }

  void assignJob(int jobid) { assigned_jobs.push_back(jobid); }
  std::string getName() const { return name; }
  std::string getType() const { return type; }

private:
  std::string name;
  std::string type;
  std::vector<int> assigned_jobs;
};

const std::map<std::string, std::vector<std::string>> TYPES2JOBS{
    {"Crane", std::vector<std::string>{"lift", "transport_materials"}},
    {"Dozer", std::vector<std::string>{"crush", "move_earth"}},
    {"Transporter", std::vector<std::string>{"transport_materials"}},
    {"Builder", std::vector<std::string>{"pour_concrete", "lay_bricks"}},
    {"Inspector", std::vector<std::string>{"inspect"}},
};
