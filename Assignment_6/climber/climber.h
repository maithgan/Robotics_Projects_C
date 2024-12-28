#include <map>

struct Coordinate
{
    int x;
    int y;
};

bool operator==(Coordinate const &c1, Coordinate const &c2);

bool operator<(Coordinate const &c1, Coordinate const &c2);

Coordinate operator+(Coordinate const &c1, Coordinate const &c2);

void read_input(std::map<Coordinate, int> &m, Coordinate &start, std::string const &infilename);

void climber_algorithm(std::map<Coordinate, int> const &m, Coordinate const &start, std::string const& outfilename);