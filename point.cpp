//211 - Kikteva Veronika
//Zadacha 1

#include "point.h"
#include <memory>
#include<cstdlib>
#include <cmath>
Point::Point(double x, double y){
    mX = x;
    mY = y;
    type = 1;
}

char * Point::draw() const{
    char * buf = (char*)malloc(100);
    sprintf(buf, "%lf %lf\n", mX, mY);
    return buf;
}

double Point::getX() const {
    return mX;
}

double Point::getY() const{
    return mY;
}

double Point::distanceTo(const Point & Other) const{
    double dx = mX - Other.mX;
    double dy = mY - Other.mY;
    return sqrt(dx*dx + dy*dy);
}

bool Point::operator==(const Point &rhs)const{
    double delt = 1e-10;
    return fabs(mX - rhs.mX) < delt && fabs(mY - rhs.mY) < delt;
}
