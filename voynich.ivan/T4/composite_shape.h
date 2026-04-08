#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H
#include "shape.h"
#include <vector>
#include <memory>

class CompositeShape : public Shape{
private:
    std::vector<std::unique_ptr<Shape>>shapes_;
public:
    CompositeShape() = default;

    void addShape(std::unique_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor);
    std::string getName() const override;

    void printShapes()const;
};

#endif

