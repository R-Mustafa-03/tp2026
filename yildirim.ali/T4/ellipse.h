#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape {
private:
    Point center;
    double rx;
    double ry;
public:
    Ellipse(const Point& c, double r_x, double r_y);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    std::string getName() const override;
    bool containsPoint(const Point& p) const override;
    double getPerimeter() const override;
    void scale(double factor) override;
};

#endif
//