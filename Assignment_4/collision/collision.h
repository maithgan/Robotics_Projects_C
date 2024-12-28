#include <eigen3/Eigen/Eigen>
#include <fstream>
#include <iostream>
#include <vector>

using Edge = std::pair<Eigen::Vector2d, Eigen::Vector2d>;

// returns true if the equation a*t^2+b*t+c=0 has a solution between 0 and 1 (inclusive)
// HINT: recall your solution to `quadratic` in HW1 
bool quadratic_has_valid_solution(double const a, double const b, double const c) {
    // --- Your code here
    if (a == 0) {
        std::cout << "\nNone" << std::endl;
        return false;
    }
    double discriminant_value = b * b - 4 * a * c; 

    double root1 = (-b + sqrt(discriminant_value)) / (2 * a); // roots of the equation
    double root2 = (-b - sqrt(discriminant_value)) / (2 * a);

    if (discriminant_value < 0) { // imaginary roots 
        return false;
    }

    return (root1 >= 0 && root1 <= 1) && (root2 >= 0 && root2 <= 1);

    // ---
}


class Disc; // forward-declare the type Disc, otherwise we can't use it in the Obstacle base class

class Obstacle
{
public:
    // returns true if the robot is collides with the obstacle
    virtual bool check_collision(Disc const &robot) const = 0;

    // returns true if whether the point p is within this disc
    virtual bool contains(Eigen::Vector2d const &p) const = 0;
};

class Disc : public Obstacle
{
public:
    Disc(double x, double y, double radius) : pos_(x, y), radius_(radius) {}

    // override check_collision (HINT: use the `norm` function!)
    bool check_collision(Disc const &robot) const override
    {
        // --- Your code here
        double center_l2 = (pos_ - robot.pos_).norm();
        if(center_l2 < (radius_ + robot.radius_) ){
            return true;
        }
        return false;
        // ---
    }

    // returns true if the point p is within this disc
    bool contains(Eigen::Vector2d const &p) const override
    {
        // --- Your code here
        double l2_dist = (p-pos_).norm();
        if(l2_dist < radius_){
            return true;
        }
        return false;
        // ---
    }


    Eigen::Vector2d pos_;
    double radius_;
};
class Rectangle : public Obstacle
{
public:
    Rectangle(double x1, double y1, double x2, double y2) : bottom_left_(x1, y1), top_right_(x2, y2),
                                                            edges_{{{x1, y1}, {x2, y1}}, {{x2, y1}, {x2, y2}}, {{x2, y2}, {x1, y2}}, {{x1, y2}, {x1, y1}}},
                                                            corners_{{x1, y1}, {x2, y1}, {x2,y2}, {x1,y2}}
    {
    }

    // override check_collision
    // HINT: use the `Rectangle::check_intersection_with_edge`, `Rectangle::contains`, and `Disc::contains` functions
    // --- Your code here
    bool check_collision(Disc const &robot) const override
        {
            //check if center of disc is inside the rectangle
            if(contains(robot.pos_)){return true;}
            //check if corners are within disc
            for(auto i:corners_){
                if(robot.contains(i)){
                    return true;
                }
            }
            for(auto i:edges_){
                    if(check_intersection_with_edge(i,robot)){return true;}
            }
            
            return false;
        }

    // ---

    // Override the `contains` function
    // --- Your code here
    bool contains(Eigen::Vector2d const &p) const override
        {
        if((p(1) > bottom_left_(1) && p(0) > bottom_left_(0)) &&  (p(0) < top_right_(0) && p(1) < top_right_(1)) ){return true;}
        return false;
        }

    // ---

    // (HINT: use the `quadratic_has_valid_solution` function)
    bool check_intersection_with_edge(Edge const &e, Disc const& disc) const
    {
        // --- Your code here
        Eigen::Vector2d Edge = e.first - e.second;//edge_vector 
        Eigen::Vector2d Rad_Edge = e.first - disc.pos_;//radius to edge_vector
        bool intersect_ =  quadratic_has_valid_solution(Edge.dot(Edge),2*Rad_Edge.dot(Edge),Rad_Edge.dot(Rad_Edge) - disc.radius_*disc.radius_);

        if(intersect_){return true;}
        return false;
        // ---
    }

    Eigen::Vector2d bottom_left_;
    Eigen::Vector2d top_right_;
    std::vector<Edge> edges_;
    std::vector<Eigen::Vector2d> corners_;
};

bool check_collisions(std::vector<Obstacle const *> const &obstacles, Disc const &robot)
{
    // --- Your code here
    for (const auto& obstacle : obstacles) {
        if (obstacle->check_collision(robot)) {
            return true;
        }
    }
    return false;

    // ---
}