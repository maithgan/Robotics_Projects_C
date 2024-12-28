#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>

int main()
{
    double a, b, c;
    std::cout << "Type in a, b, and c. Press 'Enter' between each\n";
    std::cin >> a;
    std::cin >> b;
    std::cin >> c;
    std::cout << std::setprecision(4);
    // --- Your code here
    double x1, x2;
    double discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        std::cout << "None" << std::endl;
    } 
    
    else if (discriminant == 0) {
        x1 = (-b) / (2 * a);
        if (abs(x1)==0){
            std::cout<<abs(x1)<<std::endl;
        }
        else{
            std::cout << x1 << std::endl;
        }
    } 
    
    else {
        x1 = (-b + sqrt(discriminant)) / (2 * a);
        x2 = (-b - sqrt(discriminant)) / (2 * a);
        if (x1<x2) {
            std::cout << x1 << std::endl;
            std::cout << x2 << std::endl;
        }
        else {
            std::cout << x2 << std::endl;
            std::cout << x1 << std::endl;
        }
    }
    // ---
    return 0;
}