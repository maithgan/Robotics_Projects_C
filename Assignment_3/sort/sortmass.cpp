#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "sortmass.h"

using namespace std;

int main() {
  ifstream objects("objects.txt");
  ofstream outfile("output.txt");
  string name;
  double mass;

  vector<ObjMassPair> obj_mass;
  //read the contents of the objects file in to the obj_mass vector
  // --- Your code here
  string line;
  while (getline(objects,line)){
    string word = "";
    ObjMassPair  temp;
    int i=0;
    while(line[i]!='\n' && i<line.length()){
      if (line[i] == ' ') {
        temp.first = word;
        word = "";
      }
      else
        word+= line[i];
      i++;
    }
    temp.second = std::stod(word);
    obj_mass.push_back(temp);
  }


  // ---

  //use std::sort to sort obj_mass. You will need to define
  //a function that compares ObjMassPair objects that is of
  //the form expected by std::sort

  // --- Your code here

  std::sort(obj_mass.begin(), obj_mass.end(), compare);

  // ---

  //you will need to properly overload the << operator (in sortmass.h)
  //to print obj_mass to cout
  //look at the form expected in the given output.txt
  cout << obj_mass;
  outfile << obj_mass;
}
