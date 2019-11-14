//211 - Kikteva Veronika
//Zadacha 1

#ifndef Hierarchy_h
#define Hierarchy_h
#include "../Objects/cluster.hpp"
#include "../Objects/point.h"
#include "../functional/utils.hpp"
#include <vector>
#include <algorithm>

class hierchAlgorithm{
    
    class HeavyPoint{
        //actually it stores enough info
        std::vector< Point > children;
        //Note: center will be read N times more than written so there is sense to cache it
        Point center;
        //we will update these values when updating
        //this is a pretty pattern, idk what
        
    public:
        HeavyPoint():center(0, 0){
            //just nothing
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
                return Point(p.getX() + acc.getY(), p.getY() + acc.getY());
            });
            nc.setX(nc.getX() / children.size());
            nc.setY(nc.getY() / children.size());
            return center = nc;
        }
        
        HeavyPoint Merge(HeavyPoint & Other){
            HeavyPoint new_heavy;
            for(auto point: children)
                new_heavy.addPoint(point);
            for(auto point: Other.children)
                new_heavy.addPoint(point);
            return new_heavy;
        }
        
    };
    
    //actually contain the centers
    std::vector<HeavyPoint> centers;
public:
    
    std::vector< Cluster > find(int count, std::vector<Point> points){
        std::vector<Cluster> found;
        for(auto point:points){
            HeavyPoint heavy;
            heavy.addPoint(point);
            centers.push_back(heavy);
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
        return found;
    }
    
    
    
    void Iterate(){
        //Finds least distance between two heavyPoints
        auto first = centers.begin();
        auto second = ++centers.begin();
        double min_dist = (*first).getCenter().distanceTo((*second).getCenter());
        for(auto firstIdx = centers.begin(); firstIdx < centers.end(); ++firstIdx){
            for(auto secIdx = firstIdx; secIdx < centers.end(); ++secIdx){
                double dist = (*firstIdx).getCenter().distanceTo((*secIdx).getCenter());
                if(dist!= 0 && dist < min_dist){
                    min_dist = dist;
                    first = firstIdx;
                    second = secIdx;
                }
            }
        }
        HeavyPoint newPoint = first->Merge(*second);
        centers.erase(first);
        centers.erase(second);
        centers.push_back(newPoint);
        
    }
    
    
};

#endif /* Hierarchy_h */
