
#ifndef cluster_hpp
#define cluster_hpp

#include <cstdlib>
#include "point.h"
#include <vector>



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
    void rotate( double angle );
    Point getCenter() const;
    void addPoint(const Point &);
    void print(FILE*);
    size_t size() const{
        return points.size();
    }
    std::vector<Point> & getState();
    ~Cluster(){
        
        
    }
    int getColor() const {
        return color;
    }
    char * draw() const override;
    void setColor(int);
    void archieve() const;
};





#endif /* cluster_hpp */
