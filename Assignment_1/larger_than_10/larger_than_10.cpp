#include <cmath>
#include <iostream>
#include <fstream>

int main() {
    // This time you'll have to write the print statement yourself.
    std::string filename;
    std::cout << "Enter the filename then press enter: ";
    std::cin >> filename;
    std::ifstream ifs(filename);
    int x;
    // HINT: consider using the "break" statement
    while (!ifs.eof()) {
        ifs >> x;
        // --- Your code here
        if (x>10){
            std::cout<<x;
            break;
        }
        // ---
    }
    return 0;
}