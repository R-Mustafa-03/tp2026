#pragma once
struct Point
{
    double x = 0.0;
    double y = 0.0;

    Point() = default;
    Point(double xval, double yval) : x(xval), y(yval) {};
};

