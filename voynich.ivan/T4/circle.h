#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"

class Circle : public Shape{
private:
    Point centerP;
    double radius;
public:
    Circle(const Point& center, double r);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
};

#endif

