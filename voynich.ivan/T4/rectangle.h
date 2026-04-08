#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"
#include <cmath>

class Rectangle : public Shape{
private:
    Point bottomLeft;
    Point topRight;
public:
    Rectangle(const Point& leftP, const Point& rightP);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
};

#endif

