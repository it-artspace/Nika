//
//  cluster_builder.hpp
//  task1
//
//  Created by Дмитрий Маслюков on 25/09/2019.
//  Copyright © 2019 Дмитрий Маслюков. All rights reserved.
//

#ifndef cluster_builder_hpp
#define cluster_builder_hpp

#include "cluster.hpp"
#include <vector>




class ClusterBuilder{
public:
    struct Algorithm{
        class Iterator{
        public:
            virtual void operator() (ClusterBuilder&, Canvas&) = 0;
            virtual ~Iterator() = 0;
        };
        class Determinator{
        public:
            virtual bool operator() (ClusterBuilder&, Canvas&) = 0;
            virtual ~Determinator() = 0;
        };
        
        class Binder{
        public:
            virtual void assignPoint(ClusterBuilder&, Point&) = 0;
            virtual ~Binder() = 0;
        };
        
        class AlgorithmBuilder{
            Algorithm * prototype;
        public:
            
            AlgorithmBuilder(){
                prototype = new Algorithm(0, 0);
                prototype->binder = 0;
            }
            
            AlgorithmBuilder& setIterator( Iterator * i){
                prototype->iter_func = i;
                return *this;
            }
            AlgorithmBuilder& setDeterminator( Determinator * d ){
                prototype->finalize_func = d;
                return *this;
            }
            
            AlgorithmBuilder& setBinder( Binder * b){
                prototype->binder = b;
                return *this;
            }
            
            Algorithm& build(){
                Algorithm * _algorithm = prototype;
                prototype = 0;
                return *_algorithm;
            }
        };
        Binder * binder;
        Iterator * iter_func;
        Determinator * finalize_func;
        Algorithm(Iterator * i, Determinator * d){
            iter_func = i;
            finalize_func = d;
        }
    };
    
private:
    Algorithm algorithm;
    
    std::vector<Cluster> clusters;
    Canvas * canvas;
public:
    
    Cluster & operator[](unsigned idx){
        return clusters[idx];
    }
    
    void reserve(unsigned cluster_count){
        clusters.reserve(cluster_count);
    }
    ClusterBuilder(Algorithm);
    void attach(Canvas &);
    std::vector<Cluster> launch();
    const std::vector<Cluster> & getState() const;
    void reset();
};



#endif /* cluster_builder_hpp */
