//211 - Kikteva Veornika
//Zadacha 1

#include<cstdlib>
#include "Canvas.h"

void Canvas::draw() const{
    char name[100];

    sprintf(name, "%p.txt", this);
    FILE* f = fopen(name, "w");
    fprintf(f, "plot ");
    for(IGeom * elem :children){//risuem kazhdyi
        char * dump = elem->draw(); //poluchaem strokovoe predstavlenie
        fputs(dump, f); //pishem v fail
        free(dump);
    }
    fclose(f);
}

void Canvas::createGist() const {
    FILE * f = fopen("gist.txt", "w");
    //razmechaem (0,1000) na intervaly po 50
    int regions [height / 50]; //kol-vo tochek, popavshih v interval
    for(int i = 0; i < height/50; ++i){
        regions[i] = 0;
    }
    //podschtivaem kolichectvo tochek v kazhdom
    for(IGeom * elem : children){
        //opredelyaem prinadlezhnost'
        int regionIndex = elem->getY() / 50;
        regions[regionIndex]++;
    }
    //zapisivaem v fail
    for(int i = 0; i < height / 50; ++i){
        fprintf(f, "%d\n", regions[i]);
    }
    fclose(f);
}

Point & Canvas::createPoint(double x, double y){
    Point * p = new Point(x, y);
    children.push_back(p);
    return *p;
}

Canvas::~Canvas(){
    for(IGeom * elem :children){
        delete elem;
    }
}

void Canvas::reset(){
    for(IGeom * elem :children){
        delete elem;
    }
    children.clear();
}

int Canvas::getWidth() const {
    return width;
}

int Canvas::getHeight() const{
    return height;
}
