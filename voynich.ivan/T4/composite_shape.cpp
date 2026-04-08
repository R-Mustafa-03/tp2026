#include <iostream>
#include "composite_shape.h"

void CompositeShape::addShape(std::unique_ptr<Shape> shape){
    if(!shape){
        throw std::invalid_argument("Null shape");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea()const{
    double areaSum = 0.0;
    for(size_t i = 0; i<shapes_.size(); ++i){
        areaSum += shapes_[i]->getArea();
    }
    return areaSum;
}

Point CompositeShape::getCenter()const{
    if(shapes_.empty()){
        return Point(0,0);
    }

    double sumX = 0.0;
    double sumY = 0.0;
    for(size_t i = 0; i<shapes_.size(); ++i){
        Point c = shapes_[i]->getCenter();
        sumX += c.x;
        sumY += c.y;
    }
    return Point(sumX/shapes_.size(), sumY/shapes_.size());
}

void CompositeShape::move(const Point& delta){
    for(size_t i = 0; i<shapes_.size(); ++i){
        shapes_[i]->move(delta);
    }
}

void CompositeShape::scale(double factor){
    if (factor<=0){
        throw std::invalid_argument("Factor must be positive");
    }
    if (shapes_.empty()){
        return;
    }
    Point compositeCenter = getCenter();
    for(size_t i = 0; i < shapes_.size(); ++i){
        Point shapeCenter = shapes_[i]->getCenter();
        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;

        dx *= factor;
        dy *= factor;

        Point newCenter;
        newCenter.x = compositeCenter.x + dx;
        newCenter.y = compositeCenter.y + dy;

        Point moveDelta;
        moveDelta.x = newCenter.x - shapeCenter.x;
        moveDelta.y = newCenter.y - shapeCenter.y;
        shapes_[i]->move(moveDelta);

        shapes_[i]->scale(factor);
    }
}

std::string CompositeShape::getName()const{
    return "COMPOSITE";
}

void CompositeShape::printShapes()const{
    std::cout<<"COMPOSITE, (" << getCenter().x << ", "<< getCenter().y <<"), "<< getArea() <<":"<< std::endl;
    for(size_t i = 0; i<shapes_.size(); ++i){
        Point center = shapes_[i]->getCenter();
        std::cout<< shapes_[i]->getName() << ", ("
        << center.x << ", "<< center.y <<") " << shapes_[i]->getArea();
        if(shapes_[i] != shapes_.back()){
            std::cout<< ",";
        }
        std::cout<< std::endl;
    }
}

