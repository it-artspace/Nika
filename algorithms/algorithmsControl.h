//211 - Kikteva Veronika
//Zadacha 1

#ifndef algorithmsControl_h
#define algorithmsControl_h
#include <vector>
#include <string>
#include "../Objects/cluster.hpp"
class Algorithm{
public:
    virtual void setup(char* arg)=0;
    virtual std::vector<Cluster> find(std::vector<Point>) = 0;
    const char * tip =  tip = "gnuplot with \"plot \'__arch(*)\' with using 1:2:($3) with points lc rgb variable \"";
};


//templates a function that will build the algo


#endif /* algorithmsControl_h */
