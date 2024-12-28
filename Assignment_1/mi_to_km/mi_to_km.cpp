// --- Your code here
#include <iostream>
#include<iomanip>

int main(){
    double miles, km;
    // std::cout<<"Enter the miles value that needs to get converted to km:"<< std::endl;
    std::cin>>miles;
    km=miles*1.60934;
    std::cout<<std::setprecision(3)<<km<<std::endl;
    return 0;
}



// ---