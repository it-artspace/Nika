

#ifndef spanTree_hpp
#define spanTree_hpp

#include "point.h"
#include <vector>
#include "utils.hpp"
#include <set>


class TreeFactory{
    std::set<Point> graph;
    std::set<Point> source;
    
    typedef std::pair<std::set<Point>::iterator, std::set<Point>::iterator> verge;
    typedef std::vector< verge > VergeSet;
    
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
        verge leastVerge = *std::min_element(analyzed.begin(), analyzed.end(), [&](verge v1, verge v2)->bool{
            return (*v1.first).distanceTo(*v1.second) <= v2.first->distanceTo(*v2.second);
        });
        //3
        std::set<Point>::iterator nextPoint = leastVerge.second;
        verges.push_back(leastVerge);
        graph.insert(*nextPoint);
        source.erase(nextPoint);
        /* Note: the order of erase and insertion is very important
         * we need to copy the point to the graph set before it is deleted
         */
    }
    
public:
    static double vergeLength(verge v);
    //analyzes passed set and prints gist into same named file
    void generateGist( std::set<Point> points ){
        source = points;
        //select a point
        Point p = *source.begin();
        graph.insert(p);
        source.erase(p);
        
        //consume all the points, building a graph
        printf("processing %3d%%", 0);
        fflush(stdout);
        while(!source.empty()){
            Iterate();
        }
        //now lets think about what we have achieved
        double maxDistance = vergeLength(*std::max_element(verges.begin(), verges.end(), [&](verge v1, verge v2)->bool{
            return vergeLength(v1) <= vergeLength(v2);
        }));
        //lets divide into 20 regions
        //why 20? for nice gist
        //actually also can be taken as a param and used via alloca
        int regions[20];
        bzero(regions, sizeof(int)*20);
        //every distance coming will come to dist/max * 20 position
        for(auto v: verges){
            regions[int(vergeLength(v) / maxDistance * 20)] ++;
        }
        //record
        
        FILE * f = fopen("gist.txt", "w");
        for(int i = 0; i < 20; ++i){
            fprintf(f, "%d\n", regions[i]);
        }
        fclose(f);
    }
    
};

class TreeNode{
    Point point;
    TreeNode * next;
public:
    
    
    TreeNode(Point p): point(p){}
    //extracts minimum point from the vector and returns child
    TreeNode& findNext(std::vector<Point> pointset){
        
        auto min_iterator = std::min_element(pointset.begin(), pointset.end(), [&](Point p1,Point p2)->bool{
            return p1.distanceTo(point) < p2.distanceTo(point);
        });
        TreeNode * _next = new TreeNode(*min_iterator);
        next = _next;
        pointset.erase(min_iterator);
        return *_next;
    }
    ~TreeNode(){
        TreeNode * walker = this;
        TreeNode * next;
        while(walker){
            next = walker->next;
            delete walker;
            walker = next;
        }
    }
    void gist() const{
        FILE * f = fopen("gist.txt", "w");
        
        
        //razmechaem (0,1000) na intervaly po 50
        std::vector<double> distances;
        for(auto walker = this; walker->next; walker = walker->next){
            //opredelyaem prinadlezhnost'
            distances.push_back(walker->point.distanceTo(walker->next->point));
        }
        
        double max_distance = *std::max_element(distances.begin(), distances.end());
        
        //podschtivaem kolichectvo tochek v kazhdom
        int * regions = (int*)alloca((max_distance/20 + 1)*sizeof(int));
        bzero(regions, max_distance/20*sizeof(int));
        forEach(distances, [&](double distance){
            regions[int(distance/20)]++;
        });
        //zapisivaem v fail
        for(int i = 0; i < max_distance/20; ++i){
            fprintf(f, "%d\n", regions[i]);
        }
        fclose(f);
    }
    
};

#endif /* spanTree_hpp */
