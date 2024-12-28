#include <iostream>
#include <string>

int main() {
    // you'll need to store the answer in a variable named 'sum'
    // --- Your code here
    int a=1,b=2,sum=0;
    while(a<=4000000){
        if (a%2==0){
            sum+=a;
        }
        int temp;
        temp=a+b;
        a=b;
        b=temp;
    }

    // ---
    std::cout << sum << std::endl;
    return 0;
}