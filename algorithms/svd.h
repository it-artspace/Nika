//Kikteva Veronika 211
//task1

#ifndef svd_h
#define svd_h
#include "algorithmsControl.h"
#include <vector>
#include <utility>
#include "../functional/utils.hpp"
class SVDProcessor{
    double pValue(double a11, double lambda){
        return a11 - lambda;
    }
    
    double vectorBase(double p, double q, double l){
        return sqrt( l / (p*p + q*q) );
    }
    
    Point vectorEnd(double matrix[2][2], double l){
        double p = pValue(matrix[0][0], l);
        double q = matrix[0][1];
        return Point( 2*vectorBase(p, q, l)*q, -2*p*vectorBase(p, q, l) );
    }
    
public:
    typedef std::pair<Point, Point> vector;
    std::pair<vector, vector> svdDecompose(std::vector<Point> cluster){
        double matrix[2][2];
        matrix[0][0] = matrix[1][0] = matrix[0][1] = matrix[1][1] = 0;
                
        Point center = reduceVector<Point>(cluster, [](Point p, Point acc)->Point{
            return Point(p.getX() + acc.getX(), p.getY() + acc.getY());
        });
        center.setX(center.getX() / cluster.size());
        center.setY(center.getY() / cluster.size());
        
        cluster = remapVector<Point>(cluster, [&](Point & p)->Point{
            return Point(p.getX() - center.getX(), p.getY() - center.getY());
        });
        
        forEach(cluster, [&](Point p){
            matrix[0][0] += p.getX()*p.getX();
            matrix[0][1] += p.getX()*p.getY();
            matrix[1][1] += p.getY()*p.getY();
        });
        matrix[0][0] /= cluster.size();
        matrix[0][1] /= cluster.size();
        matrix[1][1] /= cluster.size();
        matrix[1][0] = matrix[0][1];

        
        double l1, l2;
        l1 = l2 = 0.5*(matrix[0][0] + matrix[1][1]);
        double sqr = 0.5*sqrt((matrix[0][0] - matrix[1][1])*(matrix[0][0] - matrix[1][1]) + 4 *matrix[0][1]*matrix[1][0]);
        l1 -= sqr;
        l2 += sqr;
        auto firstPt = vectorEnd(matrix, l1);
        auto secPt = vectorEnd(matrix, l2);
        
        return {
            {center, firstPt},
            {center, secPt}
        };
    }
    
    
};


#endif /* svd_h */
