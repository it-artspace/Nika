//211 - Kikteva Veornika
//Zadacha 1

#ifndef Canvas_h
#define Canvas_h

#include <stdio.h>
#include <vector>
#include "point.h"

//class realizuschiy abstraciu holsta dlya vyvoda v gnuplot
class Canvas{
    //vse, chto mi zdes otobrazhaem
    const int width = 1000;
    const int height = 1000;
    std::vector<IGeom *> children; //vector iz figur
public:
    const std::vector<IGeom *>& getState() const {
        return children;
    }
    
    void addFigure(IGeom * f){
        children.push_back(f);
    }
    
    IGeom * getById(void * _id){
        //assert that there is such element
        //goal: prevent segfault
        for(auto elem : children){
            if(elem == _id)
                return elem;
        }
        return 0;
    }
    
    Point & createPoint(double x = 0, double y = 0);
    void reset();
    //pozvolyaut poluchit' pole bex izmenenia v samom objecte
    int getWidth() const;
    int getHeight() const;
    //otobrazit tekuschee sostoyanie v gnuplot
    void draw() const;
    void createGist() const;
    //pochistit pamyat za childrenyatami, kotoryh my hranim ukazatelyami
    ~Canvas();
};



#endif /* Canvas_hpp */
