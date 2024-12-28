#include <random>
#include <iomanip>
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <vector>

struct Plane
{
    double a;
    double b;
    double c;
    double d;
};

struct FitResult
{
    Plane plane;
    int n_inliers = -1;
};

void pretty_print(std::string const &input_filename, Plane const &plane)
{
    std::cout << std::setprecision(3) << "--infile " << input_filename << " -a " << plane.a << " -b " << plane.b << " -c " << plane.c << " -d " << plane.d << '\n';
}

std::ostream &operator<<(std::ostream &os, Plane const &plane)
{
    os << std::setprecision(3) << plane.a << " " << plane.b << " " << plane.c << " " << plane.d;
    return os;
}

Plane compute_plane_from_points(Eigen::Vector3d const &p0,Eigen::Vector3d const &p1,Eigen::Vector3d const &p2)
{
    // 1. given p0, p1, and p2 form two vectors v1 and v2 which lie on the plane
    // 2. use v1 and v2 to find the normal vector of the plane `n`
    // 3. set a,b,c from the normal vector `n`
    // 4. set `d = -n.dot(p0)`
    // --- Your code here
    Plane P;
    Eigen::Vector3d v1, v2, plane, pointonPlane;
    v1 = p1 - p0;
    v2 = p2 - p0;

    plane = v1.cross(v2);

    P.a = plane(0);
    P.b = plane(1);
    P.c = plane(2);
    P.d = -plane.dot(p0);
    return P;
    // ---
}

class BaseFitter
{
public:
    BaseFitter(int num_points) : mt(rd()), dist(0, num_points - 1)
    {
        mt.seed(0);
    }

    /**
     * Given all of the data `points`, select a random subset and fit a plane to that subset.
     * the parameter points is all of the points
     * the return value is the FitResult which contains the parameters of the plane (a,b,c,d) and the number of inliers
     */
    virtual FitResult fit(Eigen::MatrixXd const &points) = 0;

    int get_random_point_idx()
    {
        return dist(mt);
    };

    double const inlier_threshold_{0.02};

private:
    // These are for generating random indices, you don't need to know how they work.
    // Just use `get_random_point_idx()` and `points.row(rand_idx)`
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<int> dist;
};

class AnalyticFitter : public BaseFitter
{
public:
    AnalyticFitter(int num_points) : BaseFitter(num_points) {}

    // by writing `override`, the compiler will check that this function actually overrides something 
    // in the base class. Always use this to prevent mistakes in the function signature!
    FitResult fit(Eigen::MatrixXd const &points) override
    {
        // 1. select 3 points from `points` randomly 
        // 2. compute the equation of the plane (HINT: use compute_plane_from_points)
        // 3. compute the `n_inliers` given that plane equation
        // (HINT: multiply the points matrix by the normal vector)
        // --- Your code here

        Eigen::Matrix<double,3,3> info;
        for (int j=0;j<=2;++j){
            auto idx= get_random_point_idx();
            info.row(j)=points.row(idx);
        }
        auto analytic_plane = compute_plane_from_points(info.row(0),info.row(1),info.row(2));
        auto distance = ((points*Eigen::Vector3d{analytic_plane.a,analytic_plane.b,analytic_plane.c}).array()+analytic_plane.d).cwiseAbs()/Eigen::Vector3d{analytic_plane.a,analytic_plane.b,analytic_plane.c}.norm();
        int n_inliers=0;
        for( int i=0; i< distance.rows();i++){
            if(distance(i)<=inlier_threshold_){
                ++n_inliers;
            }
        }

        // ---

        return {analytic_plane, n_inliers};
    }
};

class LeastSquaresFitter : public BaseFitter
{
public:
    LeastSquaresFitter(int num_points, int n_sample_points) : BaseFitter(num_points), n_sample_points_(n_sample_points) {}

    // You should override the `fit` method here
    // --- Your code here
    FitResult fit(Eigen::MatrixXd const &points) override
    {
        int const n_points_ = n_sample_points_;
        std::vector<int> I;
        Eigen::MatrixXd A(n_points_, 3);
        Eigen::VectorXd b(n_points_);
        while (I.size() < n_points_) {
            int idx = get_random_point_idx();
            if (std::find(I.begin(), I.end(), idx) == I.end()) {
                I.push_back(idx);
                A.block<1,2>(I.size() - 1, 0) = points.block<1, 2>(idx, 0);
                A(I.size() - 1, 2) = 1.0;
                b(I.size() - 1) = -1 * points(idx, 2);
            }
        }

        Eigen::VectorXd coeff = (A.transpose() * A).ldlt().solve(A.transpose() * b);
        Plane analytic_plane{coeff(0), coeff(1), 1, coeff(2)};
        Eigen::Matrix<double, 3, 1> plane_coef(analytic_plane.a, analytic_plane.b, analytic_plane.c);
        Eigen::VectorXd distances = ((points * plane_coef).array() + analytic_plane.d).abs() / plane_coef.norm();
        int n_inliers = (distances.array() < inlier_threshold_).count();

        return {analytic_plane, n_inliers};
    }
    // ---

    int const n_sample_points_;
};

Plane ransac(BaseFitter &fitter, Eigen::MatrixXd const &points)
{
    // --- Your code here

    int max_inliers = 0;
    FitResult best_result;
    for (int i = 0; i < 150; ++i) { // Choose the number of iterations
        FitResult result = fitter.fit(points);
        if (result.n_inliers > max_inliers) {
            max_inliers = result.n_inliers;
            best_result = result;
        }
    }

    // ---

    // HINT: the number of inliers should be between 20 and 80 if you did everything correctly
    std::cout << best_result.n_inliers << std::endl;
    return best_result.plane;
}