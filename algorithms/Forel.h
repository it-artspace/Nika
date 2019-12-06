//211 - Kikteva Veronika
//Zadacha 1

#ifndef Forel_h
#define Forel_h
#include "algorithmsControl.h"
#include <set>
#include <vector>
#include "../Objects/cluster.hpp"
typedef std::vector<std::set<Point>::iterator> Sphere;
class ForelAlgorithm: public Algorithm{
    
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
            return Point(p->getX() + acc.getX(), p->getY() + acc.getY());
        });
        return Point( p.getX()/(sphere.size()), p.getY()/(sphere.size()) );
    }
private:
   
    void Iterate(){
        Sphere sphere;
        Point center;
        sphere.push_back(points.begin());
        do{
            center = getSphereCenter(sphere);
            sphere = getSphere(center);
            if(sphere.size() == points.size())
                break;
        }while(not (getSphereCenter(sphere) == center) );
        Cluster cl;
        for(auto& point: sphere){
            cl.addPoint(Point(*point));
            points.erase(point);
        }
        found.push_back(cl);
        
    }
public:
    void setup( char * arg ) override{
        int rad;
        sscanf(arg, "%d", &rad);
        mRadius = rad;
    }
    std::vector<Cluster> find(std::vector<Point> source) override {
        for(auto point: source){
            points.insert(point);
        }
        while( not points.empty() ){
            Iterate();
        }
        
        std::vector<Cluster> result = found;
        found.clear();
        points.clear();
        FILE * sphere_output = fopen("spheres", "w");
        forEach(result, [&](Cluster &c){
            Point center =  reduceVector<Point>(c.getState(), [&](Point p, Point acc)->Point{
                return Point(p.getX() + acc.getX(), p.getY() + acc.getY());
            });
            center.setX( center.getX() / c.size() );
            center.setY( center.getY() / c.size() );
            fprintf(sphere_output, "%lf %lf\n", center.getX(), center.getY());
        });
        fclose(sphere_output);
        return result;
    }
    
};

#endif /* Forel_h */
