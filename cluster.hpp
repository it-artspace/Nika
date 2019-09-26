//
//  cluster.hpp
//  task1
//
//  Created by Дмитрий Маслюков on 25/09/2019.
//  Copyright © 2019 Дмитрий Маслюков. All rights reserved.
//

#ifndef cluster_hpp
#define cluster_hpp

#include "point.h"
#include <vector>

#define Builder_entry(type, name) private: type m##name; public: Builder& set##name(type name){m##name = name; return *this;}


class Cluster : public IGeom{
public:
    class Builder;
private:
    //holds references to heap-allocated points
    std::vector<const Point *> points;
    const char * color;
public:
    Cluster();
    Cluster(Point & center);
    void rotate( double angle );
    Point getCenter() const;
    void addPoint(const Point &);
    void print(FILE*);
    const std::vector<const Point * > & getState() const;
    ~Cluster(){
        for(auto elem : points)
            delete elem;
        free(const_cast<char*>(color));
    }
    char * draw() const override;
    void setColor(const char *);
};





#endif /* cluster_hpp */
