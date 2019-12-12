
#include "cluster.hpp"
#include <cmath>
#include <string.h>
#include <stdlib.h>

Point Cluster::getCenter() const {
    double xCenter = 0, yCenter = 0;
    for(Point elem : points){
        xCenter += elem.getX();
        yCenter += elem.getY();
    }
    xCenter/= points.size();
    yCenter/= points.size();
    
    return Point(xCenter, yCenter);
}

void Cluster::addPoint(const Point & point){
    points.push_back(point);
}

void Cluster::print(FILE * fout){
    for(auto elem : points){
        char * pdump = elem.draw();
        fprintf(fout, "%s\n", pdump);
        free(pdump);
    }
}

void Cluster::archieve(std::string arg) const {
    FILE * f = fopen(("__arch"+arg).c_str(), "a");
    for(auto point: points){
        fprintf(f, "%lf %lf %d\n", point.getX(), point.getY(), color);
    }
    fclose(f);
}

std::vector<Point> & Cluster::getState(){
    return points;
}

Cluster::Cluster(Point & center){
    points.push_back(center);
}

Cluster::Cluster(){
    color = 0;
    type = 2;
}

void Cluster::rotate(double angle){
    // raw vector to rotate
    double vect[2];
    // restrict the center
    // another reason is the optimization to cache it and not recompute every time
    Point center = getCenter();
    for(auto & elem: points){
        vect[0] = elem.getX() - center.getX();
        vect[1] = elem.getY() - center.getY();
        double newvect[2];
        newvect[0] = cos(angle) * vect[0] + sin(angle) * vect[1];
        newvect[1] = cos(angle) * vect[1] - sin(angle) * vect[0];
        elem = Point(center.getX() + newvect[0], center.getY() + newvect[1]);
    }
}

void Cluster::setColor(int _color){
    color = _color;
}

char * Cluster::draw() const{
    char * dump = (char*)malloc(100);
    char namedump [100];
    sprintf(namedump, "%p.txt", (void*)this);
    FILE * f = fopen(namedump, "w");
    for(auto elem: points){
        char * point_dump = elem.draw();
        fprintf(f, "%s\n", point_dump);
        free(point_dump);
    }
    fclose(f);
    snprintf(dump, 100, "\"%s\" lc rgb \"#%d\",", namedump, color);
    return dump;
}
