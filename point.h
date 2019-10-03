//211 - Kikteva Veornika
//Zadacha 1

#ifndef point_h
#define point_h

#include <stdio.h>
class Canvas;
// interface dlya raznyh geometricheskih objectov, podderzhivausch`ih bazovye operacii (otrisovku)
class IGeom{
public:
    //eto abstractnyi class
    // virtual - realizacia polimorfizma v cpp (declariruet nalichie etoi funcii u naslednikov)
    virtual
    // metod draw vozvrasch'aet stroku - mi zhe risuem v gnuplot (logichnim obrazom ne menyaa ekzemplyar, poetomu const)
    char * draw() const
    { return 0; }
    const int type = 0;
    virtual double getX() const { return 0;}
    virtual double getY() const { return 0;}
    virtual ~IGeom(){}
};

//klass Point eto geometricheskaya figura - nasleduet Geom Inteface
class Point :public IGeom{
    //ego coordinati
    double mX;
    double mY;
public:
    const int type = 1;
    Point(double=0, double=0);
    double getX() const override;
    double getY() const override;
    double distanceTo(const Point &) const;
    //realizuet funciu draw is IGeom (pereopredel'aet ee dl'a tochki)
    char * draw() const override;
    ~Point() override{}
};

#endif /* point_hpp */
