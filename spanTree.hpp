

#ifndef spanTree_hpp
#define spanTree_hpp

#include "point.h"
#include <vector>
#include "utils.hpp"

class TreeNode{
    Point point;
    TreeNode * next;
public:
    
    
    TreeNode(Point p): point(p){
        next = 0;
    }
    //extracts minimum point from the vector and returns child
    TreeNode& findNext(std::vector<Point> & pointset){
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
            free(walker);
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
        int * regions = (int*)alloca(max_distance/20*sizeof(int));
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
