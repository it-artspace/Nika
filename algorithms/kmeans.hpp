//211 - Kikteva Veronika
//Zadacha 1

#ifndef kmeans_hpp
#define kmeans_hpp

#include "../Objects/cluster.hpp"
#include <vector>
#include "../functional/utils.cpp"
#include <algorithm>

//implements only finding, other code is responsible for calculating values
double count_score(std::vector<Cluster>);
class kmeansFinder{
    int k;
    std::vector<Point> centers;
    std::vector<Point> analyzed;
    
    /*
     One step of finding, generating clusters with centers in (centers)
     
     */
    std::vector<Cluster> iterate(){
        
        //built basement of clusters
        
        std::vector<Cluster> generation = remapVector< Cluster >(centers, [&](Point p)->Cluster{
            Cluster cl;
            cl.addPoint(p);
            return cl;
        });
        //
        
        forEach(analyzed, [&](Point & p){
            
            Cluster& closest = *std::min_element(generation.begin(), generation.end(), [&](Cluster & c1, Cluster & c2)->bool{
                double dist1 = c1.getCenter().distanceTo(p);
                double dist2 = c2.getCenter().distanceTo(p);
                return dist1 <= dist2;
            });
            
            closest.addPoint(p);
            
        });
        
        //seems thats all
        return generation;
    }
    
    
    
public:
    
    bool centers_remain(std::vector<Point> previous_gen){
        for(int i = 0; i < k; ++i){
            if(!(centers[i]==previous_gen[i]))
                return 0;
        }
        return 1;
    }
    
    kmeansFinder(int number){
        k = number;
    }
    
    std::vector<Cluster> find(std::vector<Point> points){
        analyzed = points;
        std::vector<Cluster> generation;
        std::vector<Cluster> prevGen;
        for(int i = 0; i < k; ++i){
            centers.push_back( points[ rand() % points.size() ] );
            generation.push_back(Cluster( points[ rand() % points.size() ] ));
        }
        do{
            prevGen = generation;
            generation = iterate();
            centers = remapVector<Point>(generation, [&](Cluster & c)->Point{
                return c.getCenter();
            });
            
        }while( centers_remain(remapVector<Point>(prevGen,
                    [&](Cluster & c)->Point{
                        return c.getCenter();
                    }))
        );
      
        return generation;
        
    }
   
    
};



#endif /* kmeans_hpp */
