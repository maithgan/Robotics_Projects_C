#include <iostream>

int euler9() {
    // --- Your code here
    int a,b,c,product=0;
    for (a=0;a<1000;a++){
        for(b=0;b<1000;b++){
            c=1000-a-b;
            if (a*a+b*b==c*c){
                product=a*b*c;
                break;
            }
        }
        if(product!=0){
            break;
        }
    }
    // ---
}

int main() {
    // This code is a lot cleaner if you write it as a function, because you can use `return` once you find the answer
    int const product = euler9();
    std::cout << product << std::endl;
    return 0;
}