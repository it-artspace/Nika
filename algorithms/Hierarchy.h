//211 - Kikteva Veronika
//Zadacha 1

#ifndef Hierarchy_h
#define Hierarchy_h
#include "../Objects/cluster.hpp"
#include "../Objects/point.h"
#include "../functional/utils.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include "algorithmsControl.h"
class hierchAlgorithm : public Algorithm{
    
    class HeavyPoint{
        //actually it stores enough info
        std::vector< Point > children;
        //Note: center will be read N times more than written so there is sense to cache it
        Point center;
        
    public:
        HeavyPoint():center(0, 0){
        }
        
        //only way to mutate is add a point
        
        std::vector<Point> getChildren() const{
            return children;
        }

        
        Point getCenter() const{
            return center;
        }
        
        //returns new center
        Point addPoint( Point p ){
            children.push_back(p);
            Point nc = reduceVector<Point>(children, [&](Point p, Point acc)->Point{
                return Point(p.getX() + acc.getX(), p.getY() + acc.getY());
            });
            nc.setX(nc.getX() / children.size());
            nc.setY(nc.getY() / children.size());
            return center = nc;
        }
        //operator +
        HeavyPoint Merge(const HeavyPoint & Other) const{
            HeavyPoint new_heavy;
            for(auto point: children)
                new_heavy.addPoint(point);
            for(auto point: Other.children)
                new_heavy.addPoint(point);
            return new_heavy;
        }
        
        bool operator < (const HeavyPoint& Other)const {
            bool factor = children.size() < Other.children.size();
            factor |= center < Other.center;
            return factor;
        }
        
    };
    
    //actually contain the centers
    std::set<HeavyPoint> centers;
    int count;
    
    
public:
    
    void setup(char * arg) override{
        sscanf(arg, "%d", &count);
    }
    std::vector< Cluster > find(std::vector<Point> points) override{
        std::vector<Cluster> found;
        for(auto point:points){
            HeavyPoint heavy;
            heavy.addPoint(point);
            centers.insert(heavy);
        }
        while(centers.size() > count){
            Iterate();
        }
        
        for(auto elem: centers){
            Cluster new_cluster;
            for(auto point: elem.getChildren()){
                new_cluster.addPoint(point);
            }
            found.push_back(new_cluster);
        }
        centers.clear();
        return found;
    }
    
    
    
    void Iterate(){
        //Finds least distance between two heavyPoints
        auto first = centers.begin();
        auto second = ++centers.begin();
        double min_dist = (*first).getCenter().distanceTo((*second).getCenter());
        for(auto firstIdx = centers.begin(); firstIdx != centers.end(); ++firstIdx){
            for(auto secIdx = firstIdx; secIdx != centers.end(); ++secIdx){
                double dist = (*firstIdx).getCenter().distanceTo((*secIdx).getCenter());
                if(fabs(dist) > 1e-15 && dist < min_dist){
                    min_dist = dist;
                    first = firstIdx;
                    second = secIdx;
                }
            }
        }
        HeavyPoint newPoint = first->Merge(*second);
        centers.erase(first);
        centers.erase(second);
        centers.insert(newPoint);
        
    }
    
    
};

#endif /* Hierarchy_h */
