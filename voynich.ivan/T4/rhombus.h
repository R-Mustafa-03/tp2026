#ifndef RHOMBUS_H
#define RHOMBUS_H
#include "shape.h"

class Rhombus : public Shape{
private:
    Point centerP;
    double vDiag;
    double hDiag;
public:
    Rhombus(const Point& center, double vD, double hD);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
};

#endif

