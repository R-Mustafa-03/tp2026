#include "Commands.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <limits>
#include <cmath>
#include <sstream>

using namespace std::placeholders;

//--------------------------------Area-----------------------------------------

double crossProduct(const Point& p1, const Point& p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

double getPolygonArea(const Polygon& pol) {
    double area = std::inner_product(
        pol.points.begin(), pol.points.end() - 1, pol.points.begin() + 1,
         0.0, std::plus<double>(), crossProduct
         );
    area += crossProduct(pol.points.back(), pol.points.front());
    return std::abs(area) / 2.0;
}

bool isEvenVertices(const Polygon& p) { return p.points.size() % 2 == 0; }
bool isOddVertices(const Polygon& p)  { return p.points.size() % 2 != 0; }
bool isNumVertices(const Polygon& p, size_t n) { return p.points.size() == n; }

double addAreaIfEven(double sum, const Polygon& p) {
    return isEvenVertices(p) ? sum + getPolygonArea(p) : sum;
}
double addAreaIfOdd(double sum, const Polygon& p) {
    return isOddVertices(p) ? sum + getPolygonArea(p) : sum;
}
double addAreaIfNum(double sum, const Polygon& p, size_t n) {
    return isNumVertices(p, n) ? sum + getPolygonArea(p) : sum;
}
double addAreaAll(double sum, const Polygon& p) {
    return sum + getPolygonArea(p);
}

void cmdArea(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "EVEN") {
        std::cout << std::fixed << std::setprecision(1)
                  << std::accumulate(polygons.begin(), polygons.end(), 0.0, addAreaIfEven) << "\n";
    } else if (arg == "ODD") {
        std::cout << std::fixed << std::setprecision(1)
                  << std::accumulate(polygons.begin(), polygons.end(), 0.0, addAreaIfOdd) << "\n";
    } else if (arg == "MEAN") {
        if (polygons.empty()) throw std::invalid_argument("");
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, addAreaAll);
        std::cout << std::fixed << std::setprecision(1) << (total / polygons.size()) << "\n";
    } else {
        size_t n = std::stoull(arg);
        if (n < 3) throw std::invalid_argument("");
        std::cout << std::fixed << std::setprecision(1)
                  << std::accumulate(polygons.begin(), polygons.end(), 0.0,
                     std::bind(addAreaIfNum, _1, _2, n)) << "\n";
    }
}

//---------------------------------Max/Min----------------------------------------

bool compareArea(const Polygon& a, const Polygon& b) {
    return getPolygonArea(a) < getPolygonArea(b);
}
bool compareVertices(const Polygon& a, const Polygon& b) {
    return a.points.size() < b.points.size();
}
void cmdMinMax(const std::vector<Polygon>& polygons, const std::string& type, const std::string& arg) {
    if (polygons.empty()) throw std::invalid_argument("");

    if (arg == "AREA") {
        auto it = (type == "MAX") ? std::max_element(polygons.begin(), polygons.end(), compareArea)
                                  : std::min_element(polygons.begin(), polygons.end(), compareArea);
        std::cout << std::fixed << std::setprecision(1) << getPolygonArea(*it) << "\n";
    } else if (arg == "VERTEXES") {
        auto it = (type == "MAX") ? std::max_element(polygons.begin(), polygons.end(), compareVertices)
                                  : std::min_element(polygons.begin(), polygons.end(), compareVertices);
        std::cout << it->points.size() << "\n";
    } else {
        throw std::invalid_argument("");
    }
}

//--------------------------------------Count-------------------------------------------

void cmdCount(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "EVEN") {
        std::cout << std::count_if(polygons.begin(), polygons.end(), isEvenVertices) << "\n";
    } else if (arg == "ODD") {
        std::cout << std::count_if(polygons.begin(), polygons.end(), isOddVertices) << "\n";
    } else {
        size_t n = std::stoull(arg);
        if (n < 3) throw std::invalid_argument("");
        std::cout << std::count_if(polygons.begin(), polygons.end(),
                     std::bind(isNumVertices, _1, n)) << "\n";
    }
}

//--------------------------------PERMS----------------------------------------

bool comparePoints(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool isPermutation(const Polygon& p1, const Polygon& p2) {
    if (p1.points.size() != p2.points.size()) return false;

    Polygon copy1 = p1;
    Polygon copy2 = p2;

    std::sort(copy1.points.begin(), copy1.points.end(), comparePoints);
    std::sort(copy2.points.begin(), copy2.points.end(), comparePoints);
    return copy1.points == copy2.points;
}

void cmdPerms(const std::vector<Polygon>& polygons, std::istream& in) {
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("");
    size_t result = std::count_if(polygons.begin(), polygons.end(),
                                  std::bind(isPermutation, _1, target));
    std::cout << result << "\n";
}

//-----------------------------------MAXSEQ-----------------------------------

struct MaxSeqFold {
    Polygon target;
    MaxSeqFold(const Polygon& t) : target(t) {}

    std::pair<int, int> operator()(const std::pair<int, int>& state, const Polygon& p) const {
        int current = (p == target) ? state.first + 1 : 0;
        return std::make_pair(current, std::max(state.second, current));
    }
};

void cmdMaxSeq(const std::vector<Polygon>& polygons, std::istream& in) {
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("");

    std::pair<int, int> result = std::accumulate(polygons.begin(), polygons.end(),
                                 std::make_pair(0, 0), MaxSeqFold(target));
    std::cout << result.second << "\n";
}

//-----------------------------------all commands---------------------------------------

void processCommands(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out) {
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command)) continue;

        try {
            if (command == "AREA") {
                std::string arg;
                if (!(iss >> arg)) throw std::invalid_argument("");
                cmdArea(polygons, arg);
            } else if (command == "MAX" || command == "MIN") {
                std::string arg;
                if (!(iss >> arg)) throw std::invalid_argument("");
                cmdMinMax(polygons, command, arg);
            } else if (command == "COUNT") {
                std::string arg;
                if (!(iss >> arg)) throw std::invalid_argument("");
                cmdCount(polygons, arg);
            } else if (command == "PERMS") {
                cmdPerms(polygons, iss);
            } else if (command == "MAXSEQ") {
                cmdMaxSeq(polygons, iss);
            } else {
                throw std::invalid_argument("");
            }

            iss >> std::ws;
            if (!iss.eof()) {
                throw std::invalid_argument("");
            }
        } catch (...) {
            out << "<INVALID COMMAND>\n";
        }
    }
}
