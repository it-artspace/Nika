

#ifndef spanTree_hpp
#define spanTree_hpp

#include "../Objects/point.h"
#include <vector>
#include <cstring>
#include "../functional/utils.hpp"
#include <set>
#include "../Objects/bitset.h"

class TreeFactory{
    std::set<const Point*> graph;
    std::set<const Point*> source;
    std::set<Point> data;
    
    
    typedef std::pair<std::set<const Point*>::iterator, std::set<const Point*>::iterator> verge;
    static double vergeLength(const verge& v);
    class vergeComp{
        public :bool operator()(const verge& v1, const verge& v2){
            return vergeLength(v1) < vergeLength(v2);
        }
    };
    
    
    typedef std::vector< verge> VergeSet;
    
    //pairs point at elements from the first set
    VergeSet verges;
    
    void Iterate(){
        //seek for the closest point to the graph
        //actually verges we need only for further analytics
        //plan:
        //1. bind the vertexes with verges
        //2. find shortest one
        //3. refer to the vertex from source (second) and add it to graph
        //sounds not too difficult
        
        
        //1
        VergeSet analyzed;
        printf("\b\b\b\b%3d%%", int(100.0* graph.size() / (graph.size() + source.size())));
        fflush(stdout);
        for(auto sourcePtIt = source.begin(); sourcePtIt!=source.end(); ++sourcePtIt){
            for(auto graphPtIt = graph.begin(); graphPtIt != graph.end(); ++graphPtIt){
                analyzed.push_back(verge(graphPtIt, sourcePtIt));
            }
        }
        //2
        const verge& leastVerge = *std::min_element(analyzed.begin(), analyzed.end(), [&](const verge& v1, const verge& v2)->bool{
            return vergeComp()(v1, v2);
        });
        //3
        std::set<const Point*>::iterator nextPoint = leastVerge.second;
        
        
        auto new_place = graph.insert(*nextPoint);
        const_cast<verge&>(leastVerge).second = new_place.first;
        verges.push_back(leastVerge);
        source.erase(nextPoint);
        /* Note: the order of erase and insertion is very important
         * we need to copy the point to the graph set before it is deleted
         */
    }
    
public:
    
    //analyzes passed set and prints gist into same named file
    void generateGist( std::set<Point> points ){
        data = points;
        for(auto& item:data){
            source.insert(&item);
        }
        //select a point
        const Point* p = *source.begin();
        graph.insert(p);
        source.erase(p);
        
        //consume all the points, building a graph
        printf("processing %3d%%", 0);
        fflush(stdout);
        while(!source.empty()){
            Iterate();
        }
        //now lets think about what we have achieved
        
        
        
        
        auto maxDistVerge = std::max_element(verges.begin(), verges.end(), [&](const verge& v1, const verge& v2)->bool{
            return vergeLength(v1) <= vergeLength(v2);
        });
        double maxDistance = vergeLength(*maxDistVerge);
        //lets divide into 20 regions
        //why 20? for nice gist
        //actually also can be taken as a param and used via alloca
        int regions[21];
        bzero(regions, sizeof(int)*21);
        //every distance coming will come to dist/max * 20 position
        for(auto v: verges){
            regions[int(vergeLength(v) / maxDistance * 20)] ++;
        }
        //record
        char spanname [1000];
        sprintf(spanname, "span%d.txt", int(clock() % 9277));
        FILE * f = fopen(spanname, "w");
        for(auto& verge: verges){
            fprintf(f, "%f %f\n%f %f\n\n", (*verge.first)->getX(), (*verge.first)->getY(), (*verge.second)->getX(), (*verge.second)->getY());
        }
        fclose(f);
    }
    
};



#endif /* spanTree_hpp */
