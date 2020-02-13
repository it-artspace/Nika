
//211 - Kikteva Veronika
//Zadacha 1

#ifndef dbscan_h
#define dbscan_h
#include "../Objects/cluster.hpp"
#include "algorithmsControl.h"
#include <vector>
#include <map>
class DBSCAN: public Algorithm{
    
    
    double mThresold;
    int minPts;
    int unusedPointQ;
    struct pointStorage{
        int classification;
        Point point;
    };
    enum Classification{
        noise = -1,
        unknown = -2
    };
    
    int currentClusterNo = 0;
    std::vector<pointStorage> mPoints;
    int currentPointIdx = 0;
    int maxPointIdx = 0;
    
    
    void Iterate(){
        for( auto& elem: mPoints ){
            if(elem.point.distanceTo( mPoints[currentPointIdx].point ) < mThresold ){
                
                
                if( elem.classification != unknown ){
                    //elem's type has higher priority as it has preceeded
                    mPoints[ currentPointIdx ].classification = elem.classification;
                } else {
                    elem.classification = mPoints[ currentPointIdx ].classification;
                }
                
            }
        }
        if( mPoints[ currentPointIdx ].classification == unknown )
            mPoints[currentPointIdx].classification = currentPointIdx;
        maxPointIdx = ++ currentPointIdx;
    }
    
    std::vector<Cluster> gather(){
        //reserve enough space
        int cluster_count = 0;
        std::map<int, int> no_to_quantity;
        
        
        for( auto& elem: mPoints ){
            if( elem.classification > 0 ){
                
                if( no_to_quantity.find( elem.classification ) == no_to_quantity.end() ){
                    no_to_quantity[ elem.classification ] = 1;
                } else {
                    no_to_quantity[ elem.classification ] += 1;
                }
                
            }
        }
        
        std::map<int, int> no_to_idx;
        
        for( auto& elem: no_to_quantity ){
            if(elem.second > minPts){
                no_to_idx [ elem.first ] = cluster_count++;
            }
        }
        
        
        
        std::vector<Cluster> result(cluster_count, Cluster());
        for( auto& storageElem: mPoints ){
            if( no_to_idx.find( storageElem.classification )!= no_to_idx.end() )
                result[ no_to_idx[ storageElem.classification ] ].addPoint( storageElem.point );
        }
        return result;
    }
    
public:
    
    void setup (char * arg) override{
        sscanf(arg, "%lf%d", &mThresold, &minPts);
    }
    std::vector<Cluster> find( std::vector<Point> points ) override{
        for(auto Point: points){
            mPoints.push_back({
                unknown,
                Point
            });
        }
        
        while( currentPointIdx < mPoints.size() ){
            Iterate();
        }
        currentClusterNo = 0;
        mPoints.clear();
        currentPointIdx = 0;
        maxPointIdx = 0;
        return gather();
    }
};

#endif /* dbscan_h */
