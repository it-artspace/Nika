//211 - Kikteva Veronika
//Zadacha 1

#ifndef Canvas_h
#define Canvas_h

#include <stdio.h>
#include <vector>
#include "point.h"


//it is like a document object model that holds first-generation children
//stateful object
class Canvas{
    //constants to specify bounds of the canvas rectangle
    const int width = 1000;
    const int height = 1000;
    std::vector<IGeom *> children; //vector iz figur
public:
    //get the state of canvas
    const std::vector<IGeom *>& getState() const {
        return children;
    }
    //update the state
    void addFigure(IGeom * f){
        children.push_back(f);
    }
    //id is the address of the element
    //it guarantees that operation will be secure
    IGeom * getById(void * _id){
        //assert that there is such element
        //goal: prevent segfault
        for(auto elem : children){
            if(elem == _id)
                return elem;
        }
        return 0;
    }
    //just same, but returns a copy of array
    const std::vector<IGeom *> getChildren() const {
        return children;
    }
    //save the state into file storage
    void save() const;
    //create a point && add it to canvas
    Point & createPoint(double x = 0, double y = 0);
    //discard all the children
    void reset();
    int getWidth() const;
    int getHeight() const;
    //gnuplot
    void draw() const;
    //creates a gistogram representation of canvas state
    void createGist() const;
    
    ~Canvas();
};



#endif /* Canvas_hpp */
