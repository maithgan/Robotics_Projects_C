#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

bool palandrome(unsigned long n) {
    std::string s = std::to_string(n);
    std::string reversed_s = s;
    std::reverse(reversed_s.begin(), reversed_s.end());
    return s == reversed_s;
}

int main() {
    // HINT: consider using std::to_string and std::reverse
    unsigned long largest_palandrome{0};
    // --- Your code here
    for (int i = 999; i >= 100; --i) {
    for (int j = i; j >= 100; --j) {  
        unsigned long no = i * j;
        if (no <= largest_palandrome) break;  
        if (palandrome(no)) {
            largest_palandrome = no;
        }
    }
}
    // ---
    std::cout << largest_palandrome << std::endl;
    return 0;
}