//211 - Kikteva Veornika
//Zadacha 1

#ifndef kmeans_hpp
#define kmeans_hpp

#include "cluster.hpp"
#include <vector>



//implements only finding, other code is responsible for calculating values
double count_score(std::vector<Cluster*>);
class kmeansFinder{
    int k;
    std::vector<Point> centers;
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
    
    std::vector<Cluster *> find(std::vector<Point *> points){
        size_t point_count = points.size();
        
        //lets select the points randomly in order to get inside the cluster
        //when point count is very big, its effective and result is extremely
        //similar to coord randoming
        //---------------
        //tech notes: we are to pass everywhere just pointers to points
        //they (points) are already allocated in canvas so copies dont need
        //to be store somewhere else
        
        std::vector<Cluster *> clusters;
        
        for(int pointno = 0; pointno < k; ++pointno){
            centers.push_back( *points[ rand() %  point_count] );
        }
        std::vector<Point> this_gen;
        
        do{
            //firstly rewind the clusters
            for(int i = 0; i < k; ++i){
                clusters[i]->getState().clear();
                clusters[i]->getState().push_back( &centers[i] );
            }
            for(auto& point: points){
                
                
                /*
                Cluster * belongs_to = *std::min(clusters.begin(), clusters.end(), [](Cluster * c)->double{
                        return c->getCenter().distanceTo(*point);
                    }
                );*/
                Cluster * belongs_to = clusters[1];
                double min_distance = belongs_to->getCenter().distanceTo(*point);
                for(int i = 1; i < k; ++i){
                    double d = clusters[i]->getCenter().distanceTo(*point);
                    if(d < min_distance){
                        belongs_to = clusters[i];
                    }
                }
                belongs_to->getState().push_back(point);
            }
            //then recount the centers and save them
            for(int i = 0; i < k; ++i){
                this_gen.push_back( clusters[i]->getCenter() );
            }
            //we need to reset centers after checking the condition
            /* how does this work: in compound operator the result is the last
             value (surprisingly placed on stack) so the expression is
                __remains_cond__ && true
             even more unexpectable it always equals to __remains_cond__
             but has desired side effects
             */
        }while( centers_remain(this_gen) && ({centers = this_gen; true;}) );
        return clusters;
    }
    // to use std::min
    // standard iterator class implementation
    // kmeansFinder is not a collection, however it can be named a lazy one
    // it provides Iterable interface over k giving as result the formula
    //{given summing formula}
    
    class Iterator : public std::forward_iterator_tag{
        int k_current;
        double val;
        std::vector<Point *> points;
    public:
        Iterator(){}
        Iterator(int k, std::vector<Point*> p){
            k_current = k;
            points = p;
        }
        Iterator(const Iterator& rhs){
            k_current = rhs.k_current;
            points = rhs.points;
        }
        Iterator& operator=(const Iterator & rhs){
            k_current = rhs.k_current;
            points = rhs.points;
            return *this;
        }
        
        Iterator& operator++(){
            k_current ++;
            return *this;
        }
        Iterator& operator++(int){
            k_current ++;
            return *this;
        }
        double& operator*(){
            kmeansFinder f( k_current );
            return val = count_score( f.find( points ) );
        }
        
        int getK()const{
            return k_current;
        }
        
        bool operator!=(Iterator&rhs){
            return k_current != rhs.k_current;
        }
        double* operator->(){
            return &val;
        }

    };
    /*we will stop iteration when k equals points.size() as it is kinda nonsense*/
    static Iterator startFind(std::vector<Point*> points){
        Iterator it(0, points);
        return it;
    }
    
};



#endif /* kmeans_hpp */
