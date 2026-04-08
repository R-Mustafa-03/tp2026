#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include <string>
#include <stdexcept>

class Shape{
public:

    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(const Point& delta) = 0;
    virtual void scale(double factor) = 0;
    virtual std::string getName() const = 0;
    virtual ~Shape() = default;
};

#endif

