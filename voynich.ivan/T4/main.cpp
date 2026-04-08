#include <iostream>
#include <memory>
#include <iomanip>
#include <vector>

#include "point.h"
#include "circle.h"
#include "composite_shape.h"
#include "rectangle.h"
#include "rhombus.h"

void printInfo(const std::vector<std::unique_ptr<Shape>>& shapes){
    for(const auto& shape : shapes){
        if (shape->getName() == "COMPOSITE"){
            const CompositeShape* comp = dynamic_cast<CompositeShape*>(shape.get());
            comp->printShapes();
        }
        else{
            Point center = shape->getCenter();
            std::cout<<"[" << shape->getName() << ", (" << center.x << ", "<< center.y
            <<"), " <<shape->getArea() << "]\n";
        }
    }
}

int main(){
    std::cout<<std::fixed<<std::setprecision(2);

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(3.0, 4.0)));
    shapes.push_back(std::make_unique<Circle>(Point(-5.0, -3.0), 5.0));
    shapes.push_back(std::make_unique<Rhombus>(Point(5.0, -3.0), 3.0, 4.0));
    shapes.push_back(std::make_unique<Circle>(Point(8.0, 3.0), 4.0));
    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(Point(10.0, 4.0), Point(14.0, 8.0)));
    composite->addShape(std::make_unique<Circle>(Point(10.0, -10.0), 6.0));
    shapes.push_back(std::move(composite));

    std::cout<<"Before scale:\n";
    printInfo(shapes);


    double k = 2.0;
    for(auto& shape: shapes){
        shape->scale(k);
    }
    std::cout<<"After scale:\n";
    printInfo(shapes);
}

