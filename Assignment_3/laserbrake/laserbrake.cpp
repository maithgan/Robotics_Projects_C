#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

double DEG2RAD = M_PI / 180; //this definition will be useful

using namespace std;

int main() {
  ifstream scanfile("input.txt");
  ofstream outfile("output.txt");
  cout << setprecision(4); //this sets how many decimal places to print
  outfile << setprecision(4);

  
  double robot_width;
  double threshold;

  string input;
  for(int i=0; i < 2; i++)
  {
    scanfile >> input;
    if (input == "width:")
      scanfile >> robot_width;
    else if (input == "threshold:")
      scanfile >> threshold;
  }
  cout << "Width: " << robot_width << ", Threshold: " << threshold << endl;
  double temp;
  vector<double> scan_returns;
  while (scanfile >> temp) {
    scan_returns.push_back(temp);
    cout << scan_returns[scan_returns.size()-1] << " ";
  }
  cout << endl;
  
  double resolution =
      180.0 / (scan_returns.size() - 1); // assume last scan is at 180
  cout << "Resolution: " << resolution << " degrees" << endl;
  

  // convert each scan return to x,y coordinates, and write to output.txt
  // --- Your code here
  std::vector<std::pair<double, double>> scan_coordinates;
  for (int i = 0; i < scan_returns.size(); i++) {
    scan_coordinates.push_back(std::pair<double, double> (scan_returns[i] * (cos(i * resolution * M_PI / 180)), scan_returns[i] * (sin(i * resolution * M_PI / 180))));
    outfile << scan_returns[i] * (cos(i * resolution * M_PI / 180)) << " " << scan_returns[i] * (sin(i * resolution * M_PI / 180)) << std::endl;
  }


  // ---

  //check if each point is 1) in front of the robot and 
  //2) closer than the threshold (in y)
  // --- Your code here
  bool flag = false;
  int path;
  for (int i=0; i < scan_coordinates.size(); i++) {
    double x_coord = scan_coordinates[i].first;
    double y_coord = scan_coordinates[i].second;
    if ((x_coord >= (-1 * (robot_width/2.0)) && x_coord <= (robot_width/2.0)) && (scan_returns[i] <= threshold)) { 
      flag = true;
      path = i;
      break;
    }
  }
  if (flag)
    outfile << "Stop! " << path << std::endl;
  else
    outfile << "OK" << std::endl;

  // ---
  return 0;
}
