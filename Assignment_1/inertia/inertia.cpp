#include <cmath>
#include <iostream>
#include <fstream>

int main()
{
    double mass, w, h, d;
    std::ifstream ifs("inertia_input.txt");
    ifs >> mass;
    ifs >> w;
    ifs >> h;
    ifs >> d;
    // you'll need to store the outputs in variables Ih, Iw, and Id.
    // --- Your code here
    //Inertial formula
    double Ih,Iw,Id;
    Ih=(mass*(pow(d,2)+pow(w,2)))/12;
    Id=(mass*(pow(w,2)+pow(h,2)))/12;
    Iw=(mass*(pow(d,2)+pow(h,2)))/12;

    // ---
    std::cout << "<ixx>" << Ih << "</ixx>\n";
    std::cout << "<iyy>" << Iw << "</iyy>\n";
    std::cout << "<izz>" << Id << "</izz>\n";
    return 0;
}