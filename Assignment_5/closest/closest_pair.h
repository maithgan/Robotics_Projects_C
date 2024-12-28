#include <vector>
#include <math.h>
#include <algorithm>

struct Point {
    int id;
    double x, y;
};

double dist(const Point& p1, const Point& p2) {
    auto dx = p1.x - p2.x;
    auto dy = p1.y - p2.y;
    return std::sqrt(dx*dx + dy*dy);
}

// --- Your code here
bool comparison(const Point& pointA, const Point& pointB) {
    if (pointA.x != pointB.x) {
        return pointA.x < pointB.x;
    }
    return pointA.y < pointB.y;
}

std::pair<Point, Point> bruteforce(const std::vector<Point>& pointList) {
    double minDistance = std::numeric_limits<double>::infinity();
    std::pair<Point, Point> closestPair;
    for (size_t i = 0; i < pointList.size(); ++i) {
        for (size_t j = i + 1; j < pointList.size(); ++j) {
            double distance = dist(pointList[i], pointList[j]);
            if (distance < minDistance) {
                minDistance = distance;
                if (pointList[i].id < pointList[j].id) {
                    closestPair = {pointList[i], pointList[j]};
                } else {
                    closestPair = {pointList[j], pointList[i]};
                }
            }
        }
    }
    return closestPair;
}

std::pair<Point, Point> recursive(std::vector<Point>& pointsSortedByX) {
    if (pointsSortedByX.size() <= 3) {
        return bruteforce(pointsSortedByX);
    }

    std::vector<Point> sortedPointsX = pointsSortedByX;
    std::sort(sortedPointsX.begin(), sortedPointsX.end(), comparison);

    size_t midIndex = sortedPointsX.size() / 2;
    std::vector<Point> leftPoints(sortedPointsX.begin(), sortedPointsX.begin() + midIndex);
    std::vector<Point> rightPoints(sortedPointsX.begin() + midIndex, sortedPointsX.end());

    auto closestPairLeft = recursive(leftPoints);
    auto closestPairRight = recursive(rightPoints);

    double leftDistance = dist(closestPairLeft.first, closestPairLeft.second);
    double rightDistance = dist(closestPairRight.first, closestPairRight.second);

    double minDist = std::min(leftDistance, rightDistance);
    auto closestPair = (leftDistance < rightDistance) ? closestPairLeft : closestPairRight;

    Point middlePoint = sortedPointsX[midIndex];
    std::vector<Point> strip;
    for (const auto& point : sortedPointsX) {
        if (std::abs(point.x - middlePoint.x) < minDist) {
            strip.push_back(point);
        }
    }

    std::sort(strip.begin(), strip.end(), [](const Point& a, const Point& b) { return a.y < b.y; });

    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minDist; ++j) {
            double distance = dist(strip[i], strip[j]);
            if (distance < minDist) {
                minDist = distance;
                if (strip[i].id < strip[j].id) {
                    closestPair = {strip[i], strip[j]};
                } else {
                    closestPair = {strip[j], strip[i]};
                }
            }
        }
    }
    return closestPair;
}
// ---

std::pair<Point, Point> closest_pair(const std::vector<Point>& points) {
    // --- Your code here
    std::vector<Point> pointsSortedByX = points;
    std::sort(pointsSortedByX.begin(), pointsSortedByX.end(), [](const Point& pointA, const Point& pointB) { return pointA.x < pointB.x; });
    
    return recursive(pointsSortedByX);
    // ---
}