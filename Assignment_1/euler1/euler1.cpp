#include <iostream>
#include <string>

int main() {
    unsigned long answer = 0;
    // --- Your code here

    for (int i=2;i<1000;i++){
        if (i%3==0 || i%5==0){
            answer+=i;
        }
        else{
            continue;
        }
    }
    // ---
    std::cout << answer << std::endl;
    return 0;
}