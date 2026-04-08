#include "rhombus.h"

Rhombus::Rhombus(const Point& center, double vD, double hD): centerP(center){
    if(vD <= 0 || hD <= 0){
        throw std::invalid_argument("Diagonals must be possitive");
    }
    else{
        vDiag = vD;
        hDiag = hD;
    }
};


double Rhombus::getArea()const{
    double area = (vDiag * hDiag) / 2.0;
    return area;
}

Point Rhombus::getCenter()const{
    return centerP;
}

void Rhombus::move(const Point& delta){
    centerP.x += delta.x;
    centerP.y += delta.y;
}

void Rhombus::scale(double factor){
    if (factor<=0){
        throw std::invalid_argument("Factor must be positive");
    }
    vDiag *= factor;
    hDiag *= factor;
}

std::string Rhombus::getName()const{
    return "Rhombus";
}

