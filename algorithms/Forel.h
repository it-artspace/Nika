//
//  Forel.h
//  task1
//
//  Created by Дмитрий Маслюков on 29.11.2019.
//  Copyright © 2019 Дмитрий Маслюков. All rights reserved.
//

#ifndef Forel_h
#define Forel_h
#include "../Objects/cluster.hpp"
typedef std::vector<std::set<Point>::iterator> Sphere;
class ForelAlgorithm{
    
    std::set<Point> points;
    std::vector<Cluster> found;
    int mRadius;
    
    Sphere getSphere(Point center){
        Sphere result;
        for(auto pit = points.begin(); pit != points.end(); ++pit){
            if( center.distanceTo(*pit) < mRadius ){
                result.push_back(pit);
            }
        }
        return result;
    }
public:
    
    Point getSphereCenter(Sphere sphere){
        Point p = reduceVector<Point>(sphere, [&](std::set<Point>::iterator p, Point acc)->Point{
            return Point(p->getX() + acc.getY(), p->getY() + acc.getY());
        });
        return Point( p.getX()/sphere.size(), p.getY()/sphere.size() );
    }
private:
    
    void Iterate(){
        Sphere sphere;
        Point center;
        sphere.push_back(points.begin());
        do{
            center = getSphereCenter(sphere);
            sphere = getSphere(center);
        }while(not (getSphereCenter(sphere) == center) );
        Cluster cl;
        for(auto& point: sphere){
            cl.addPoint(Point(*point));
            points.erase(point);
        }
        found.push_back(cl);
        
    }
public:
    std::vector<Cluster> find(std::vector<Point> source, int radius){
        for(auto point: source){
            points.insert(point);
        }
        mRadius = radius;
        while( not points.empty() ){
            Iterate();
        }
        return found;
    }
    
};

#endif /* Forel_h */
