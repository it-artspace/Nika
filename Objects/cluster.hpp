
#ifndef cluster_hpp
#define cluster_hpp

#include <cstdlib>
#include "point.h"
#include <vector>
#include <string>

//cluster is a array-like object that holds points and can be used to modify that
// is a geometric object so can be held in canvas
class Cluster : public IGeom{
public:
    class Builder;
private:
    //holds references to heap-allocated points
    std::vector<Point> points;
    //rgb
    int color;
public:
    Cluster();
    Cluster(Point & center);
    //rotate the cluster around its center
    void rotate( double angle );
    //count the arithmetic center of the cluster
    Point getCenter() const;
    // add a point to a cluster
    void addPoint(const Point &);
    //print a cluster to a file
    void print(FILE*);
    size_t size() const{
        return points.size();
    }
    //get underlying state
    std::vector<Point> & getState();
    ~Cluster(){
        
        
    }
    int getColor() const {
        return color;
    }
    char * draw() const override;
    void setColor(int);
    void archieve(std::string arg="") const;
};





#endif /* cluster_hpp */
