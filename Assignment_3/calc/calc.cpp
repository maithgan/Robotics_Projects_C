#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
// --- Your code here

using namespace std;

void line_process(const string &l, ofstream& outfile){
  istringstream input(l);
  vector<string> numbers_oper;
  vector<double> numbers;
  string operator_symbol;
  string s;
  double num;
  //bool flag= false;

  while (getline(input,s,' ')){
    numbers_oper.push_back(s);
  }
  operator_symbol=numbers_oper.back();
  numbers_oper.pop_back();
  for (const string &str: numbers_oper){
    numbers.push_back(std::stod(str));
  }

  //std::cout<<operator_symbol<<endl;
  if (operator_symbol == "*") {
    double product = 1;
    for (double n : numbers) {
      product *= n;
    }
    outfile<<product<<endl;
  }

  else if (operator_symbol=="+"){
    double sum=0;
    //flag=true;
    for (double n : numbers) {
      sum += n;
    }
    //cout<<sum<<endl;
    outfile<<sum<<endl;
  } 
  
  else if (operator_symbol == "/") {
    if ((numbers.size() == 2) && (numbers[1]!=0)) {
      double result = numbers[0] / numbers[1];
      outfile<<result<<endl;
    } 
    else {
      outfile<<"ERROR"<<endl;
    }
  }
  else {
    outfile<<"ERROR"<<endl;
  }
}

// ---
int main()
{
  ifstream infile("input.txt");
  ofstream outfile("output.txt");

  //Read in each line from the input file,
  //perform the specified operation on the numbers,
  //and write the results to the ouputfile.
  //Remember to check if the specified operation is possible!
  // --- Your code here
  string line;
  while (getline(infile,line)){
    line_process(line,outfile);
  }

  infile.close();
  outfile.close();

  // ---
  return 0;
}
