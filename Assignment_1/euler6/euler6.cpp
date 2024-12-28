#include <iostream>

int main() {
    // --- Your code here
    signed long sum_of_squares=0, square_of_sum=0;
    for (int i=0;i<=100;i++){
        sum_of_squares+=i*i;
        square_of_sum+=i;
    }
    square_of_sum=square_of_sum*square_of_sum;

    long difference = square_of_sum-sum_of_squares;
    // ---
    std::cout << difference << std::endl;
    return 0;
}