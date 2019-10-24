////211 - Kikteva Veronika
//Zadacha 1

#ifndef Group_hpp
#define Group_hpp

#include "point.h"
#include <stdlib.h>
#include <vector>
class Group{
    std::vector<Point> points;
    int bounds[4];
    int amount;
    int disp;
public:
    Group(int _d, int _a, int * _bounds){
        memcpy(bounds, _bounds, sizeof(int)*4);
        amount = _a;
        disp = _d;
    }
    std::vector<Point> generate(){
        for(int i = 0; i < amount; ++i){
            double x = 0, y = 0;
            for(int j = 0; j < disp; ++j){
                x += rand()%(bounds[2] - bounds[0]) + bounds[0];
                y += rand()%(bounds[3] - bounds[1]) + bounds[1];
            }
            x/=disp;
            y/=disp;
            points.push_back(Point(x, y));
        }
        return points;
    }
};

#endif /* Group_hpp */
