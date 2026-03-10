#pragma once
#include"Shape.h"
class Rectangle: public Shape
{
private:
    Point bottomLeft;
    Point topRight;
public:
    Rectangle(const Point& bl, const Point& tr);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;

};

