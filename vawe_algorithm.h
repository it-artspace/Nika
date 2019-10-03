//
//  vawe_algorithm.h
//  task1
//
//  Created by Дмитрий Маслюков on 02/10/2019.
//  Copyright © 2019 Дмитрий Маслюков. All rights reserved.
//

#ifndef vawe_algorithm_h
#define vawe_algorithm_h

#include "cluster.hpp"

class clusterFinder{
public:
    static
    std::vector<Cluster> vaweSearch(std::vector<Point *> points, double treshold){
        std::vector<Cluster> clusters;
        std::vector<std::vector<bool>> matrix;
        matrix.reserve(points.size());
        for(auto& elem: matrix){
            elem.reserve(points.size() + 1);
            elem[points.size()] = true;
        }
        for(int first_point_pos = 0; first_point_pos < points.size(); ++ first_point_pos){
            for(int second_point_pos = 0; second_point_pos < points.size(); ++ second_point_pos){
                double distance = points[first_point_pos]->distanceTo(*points[second_point_pos]);
                matrix[first_point_pos][second_point_pos] = distance > treshold;
            }
        }
        for(int point_iterator = 0; point_iterator < points.size(); ++point_iterator){
            if(matrix[point_iterator][points.size()]){
                Cluster cluster;
                cluster.addPoint(*points[point_iterator]);
                for(int points_within = point_iterator + 1; points_within < points.size(); ++points_within){
                    if(matrix[point_iterator][points_within] && matrix[points_within][points.size()]){
                        cluster.addPoint(*points[points_within]);
                        matrix[points_within][points.size()] = false;
                    }
                }
                clusters.push_back(cluster);
            }
            
        }
        return clusters;
    }
};



#endif /* vawe_algorithm_h */
