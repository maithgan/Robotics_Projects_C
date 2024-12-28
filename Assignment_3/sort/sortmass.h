#include <iostream>
#include <vector>

using ObjMassPair = std::pair<std::string, double>; //this defines the type ObjMassPair to be std::pair<std::string, double>

//define the operator << here
//define a function that compares ObjMassPair objects here (for use by std::sort)
// --- Your code here
bool compare(ObjMassPair a, ObjMassPair b){
    return a.second < b.second;
}

std::ostream& operator<<(std::ostream& out, std::vector<ObjMassPair> objmass) {
    for (ObjMassPair n : objmass)
        out << n.first << " " << n.second << std::endl;   
    return out;
}


// ---
