
#ifndef ClusterBuilder_h
#define ClusterBuilder_h
#include "../Objects/cluster.hpp"
#include <ctime>
class Cluster::Builder{
    Cluster * cluster;
    int minH, minW, maxH, maxW;
    int mPointAmount;
    int disp;
    
    
    class DispGenerator{
    public:
        static int generate(int disp, int min, int max){
            int accumulated = 0;
            for(int generation = 0; generation < disp; ++generation){
                accumulated += rand() % (max - min) + min;
            }
            return accumulated / disp;
        }
    };
    
public:
    Builder(){
        
        cluster = new Cluster();
        minH = minW = maxH = maxW = disp = 0;
    }
    
    Cluster& build(){
        if(minW == maxW || minH == maxH){
            //inappropriate range
            throw "Inappropriate range";
        }
        if(mPointAmount == 0){
            throw "Inappropriate amount";
        }
        for(int i = 0; i < mPointAmount; ++i){
            Point * p = new Point(
                                  DispGenerator::generate(disp, minW, maxW),
                                  DispGenerator::generate(disp, minH, maxH)
                                  );
            cluster->addPoint(*p);
        }
        return *cluster;
    }
    
    Builder& setBounds(int bounds[4]){
        minH = bounds[0];
        minW = bounds[1];
        maxH = bounds[2];
        maxW = bounds[3];
        return *this;
    }
    
    Builder& setDisp(int _disp){
        disp = _disp;
        return *this;
    }
    
    Builder& setPointAmount(int amount){
        mPointAmount = amount;
        return *this;
    }
    
};
#endif /* ClusterBuilder_h */
