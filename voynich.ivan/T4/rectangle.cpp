#include "rectangle.h"

Rectangle::Rectangle(const Point& leftP, const Point& rightP){
    if(rightP.x < leftP.x || rightP.y < leftP.y){
        throw std::invalid_argument("Right point smaller then left");
    }
    else{
        bottomLeft = leftP;
        topRight = rightP;
    }
};

double Rectangle::getArea()const{
    double width = topRight.x - bottomLeft.x;
    double hight = topRight.y - bottomLeft.y;
    return std::abs(width*hight);
}

Point Rectangle::getCenter()const{
    Point center;
    center.x = (bottomLeft.x + topRight.x) / 2.0;
    center.y = (bottomLeft.y + topRight.y) / 2.0;
    return center;
}

void Rectangle::move(const Point& delta){
    bottomLeft.x += delta.x;
    topRight.x += delta.x;
    bottomLeft.y += delta.y;
    topRight.y += delta.y;
}

void Rectangle::scale(double factor){
    if (factor<=0){
        throw std::invalid_argument("Factor must be positive");
    }
    Point center = getCenter();

    bottomLeft.x = center.x + (bottomLeft.x - center.x) * factor;
    bottomLeft.y = center.y + (bottomLeft.y - center.y) * factor;
    topRight.x = center.x + (topRight.x - center.x) * factor;
    topRight.y = center.y + (topRight.y - center.y) * factor;
}

std::string Rectangle::getName()const{
    return "RECTANGLE";
}

