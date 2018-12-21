#include "Utils.hpp"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <set>
#include <map>

struct Point {
    int x, y;

    int distance_from(int X, int Y) const {
        return std::abs(x-X) + std::abs(y-Y);
    }
};

bool operator<(const Point& point1, const Point& point2) {
    return std::tie(point1.x, point1.y) < std::tie(point2.x, point2.y);
}

std::vector<Point> ParseInput(const std::vector<std::string>& lines) {
    auto result = std::vector<Point>();

    for (auto& line : lines) {
        std::stringstream ss(line);

        int x, y;
        ss >> x;
        ss.ignore(2);
        ss >> y;

        result.emplace_back(Point {x, y});
    }

    return result;
}

std::pair<int, int> FindBounds(const std::vector<Point>& points) {
    auto max_x = -1, max_y = -1;

    for (auto& point : points) {
        max_x = std::max(max_x, point.x);
        max_y = std::max(max_y, point.y);
    }

    return {max_x, max_y};
}

const Point* FindNearestPoint(const std::vector<Point>& points, int x, int y) {
    auto min_dist = 1'000'000;
    const Point* nearest = nullptr;

    for (auto& point : points) {
        auto dist = point.distance_from(x, y);
        if (dist == min_dist) {
            nearest = nullptr;
        } else if (dist < min_dist) {
            min_dist = dist;
            nearest = &point;
        }
    }

    return nearest;
}

int PointWithLargestFiniteArea(const std::map<Point, int>& areas, std::set<Point>& infinte_areas) {
    auto largest_area = -1;
    for (auto& [point, area] : areas) {
        if (infinte_areas.find(point) != infinte_areas.end()) {
            continue;
        }
        largest_area = std::max(largest_area, area);
    }
    return largest_area;
}

int SolvePart1(const std::vector<Point>& points) {
    auto [max_x, max_y] = FindBounds(points);

    // an extreme point is a point that lies in an infinite region
    auto is_extreme_point = [&](int x, int y) {
        return x == 0 || x == max_x || y == 0 || y == max_y;
    };

    auto infinite_neighbourhoods = std::set<Point>();
    auto neighbourhood_sizes = std::map<Point, int>();

    for (auto x = 0; x <= max_x; ++x) {
        for (auto y = 0; y <= max_y; ++y) {
            auto nearest_point = FindNearestPoint(points, x, y);
            
            if (nearest_point == nullptr) { continue; }
            
            if (is_extreme_point(x, y)) {
                infinite_neighbourhoods.insert(*nearest_point);
            }

            ++neighbourhood_sizes[*nearest_point];
        }
    }
    
    return PointWithLargestFiniteArea(neighbourhood_sizes, infinite_neighbourhoods);
}

int SumDistancesToPoints(const std::vector<Point>& points, int x, int y) {
    return std::accumulate(points.begin(), points.end(), 0,
        [&](int current, const Point& point) {
            return current + point.distance_from(x, y);
        });
}

int SolvePart2(const std::vector<Point>& points) {
    auto [max_x, max_y] = FindBounds(points);
    auto distance_limit = 10'000;
    auto shared_region_size = 0;

    for (auto x = 0; x <= max_x; ++x) {
        for (auto y = 0; y <= max_y; ++y) {
            auto dist_sum = SumDistancesToPoints(points, x, y);

            if (dist_sum < distance_limit) {
                ++shared_region_size;
            }
        }
    }

    return shared_region_size;
}

int main() {
    auto lines = aoc::ReadLines("Day6.txt");
    auto points = ParseInput(lines);

    auto solution1 = SolvePart1(points);
    auto solution2 = SolvePart2(points);
    
    std::cout << "Solution 1:" << solution1 << std::endl;
    std::cout << "Solution 2:" << solution2 << std::endl;

    return 0;
}
