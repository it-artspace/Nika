//211 - Kikteva Veronika
//Zadacha 1

#ifndef Hierarchy_h
#define Hierarchy_h
#include "cluster.hpp"

class hierchAlgorithm{
    
    class HeavyPoint: public Point{
        int weight;
    public:
        HeavyPoint(double x=0, double y=0):Point(x, y){
            weight = 1;
        }
        
        int getWeight() const {
            return weight;
        }
        
        void setWeight(int w){
            weight = w;
        }
    };
    
    std::vector< std::vector<double> > distanceMatrix;
    std::vector< bool > avaliablePoints;
    std::vector< HeavyPoint > points;
    
    
    int getMinDistanceIndex(){
        //search for first available index
        int minIdx = 0;
        while( !avaliablePoints[minIdx] )
            minIdx++;
        //
        for(int i = 0; i < distanceMatrix.size(); ++i){
            
        }
        return 0;
    }
    
    void Iterate(){
        
        
        
        
    }
    
    
};

#endif /* Hierarchy_h */
