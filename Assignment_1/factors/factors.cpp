#include <iostream>

int main() {
    // determines whether a number (from stdin) is a factor of 2 or 3
    // the answer should be stored in a variable `is_factor`. Printing of this variable
    // is handled for you, but you must create the variable, or you'll get a compiler error.
    int x;
    std::cout << "Type an integer then press Enter: ";
    std::cin >> x;
    // --- Your code here
    bool is_factor;
    if (x%2==0|| x%3==0){
        is_factor=true;
    }
    else{
        is_factor=false;
    }
    // ---
    std::cout << is_factor << std::endl;
    return 0;
}