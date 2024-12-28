#include <iostream>
#include <fstream>
// below are some includes that are hints for what could make things easier
#include <string>
#include <vector>
#include <algorithm>

void processLine(std::string& line, int lineNumber, int* counter) {
    // line is passed in by reference, so you can modify it directly
    // do not return anything from this function
    // --- Your code here
    int sum_line=0;
    for (int i=0; i<line.size(); i++){
        if (line[i]=='e'){
            line[i]='3';
            sum_line+=3;
        }
        if (line[i]=='l'){
            line[i]='1';
            sum_line+=1;
        }
        if (line[i]=='t'){
            line[i]='7';
            sum_line+=7;
        }
    }
    *counter = *counter + (sum_line*lineNumber);
    // ---
}

int* createCounter() {
    // this is a contrived function to get you more familiar with heap allocation
    // if you return a pointer to a local variable (on the stack), that value will be invalid after the return
    // and accessing it will be unexpected behavior (probably a segfault)
    // remember to initialize it to 0
    // --- Your code here
    int *c = new int{0}; //creating a heap integer count variable
    return c;
    // ---
}

int main() {

    // you will need to read the API of std::ofstream and std::ifstream
    std::ifstream input{"input.txt"};
    std::vector<std::string> lines;
    std::string line;

    // create a reference to the pointed value
    // references can be used more conveniently than pointers
    // note that the processLine function takes counter as a pointer, so you have to address it with &
    // contrived example to get you more familiar with going between references and pointers
    // usually there will not be so many unnecessary conversions
    int& counter = *createCounter();

    // --- Your code here
    int countLine=0;
    while (getline(input,line)){
        int sum_line=0;
        processLine(line,countLine,&counter);
        lines.push_back(line);
        countLine=countLine+1;
    }


    // ---

    // output
    // --- Your code here
    std::ofstream file;
    file.open ("output.txt");
    for (int i=lines.size()-1;i>=0;i--){
        file<<lines[i]<<std::endl;
    }
    file<<counter<<std::endl;
    file.close();


    // ---

    delete &counter; //free storage of heap
}