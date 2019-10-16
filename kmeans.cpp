//211 - Kikteva Veornika
//Zadacha 1
#include "kmeans.hpp"
double count_score(std::vector<Cluster *> clusters){
    double score = 0;
    for(int i = 0; i < clusters.size(); ++i){
        
        for(int point1 = 0; point1 < clusters[i]->size(); ++point1){
            for(int point2 = point1 + 1; point2 < clusters[i]->size(); ++point2){
                score += clusters[i]->getState()[point1]->distanceTo(*clusters[i]->getState()[point2]);
            }
        }
        
        for(int clust2 = i+1; clust2 < clusters.size(); ++clust2){
            score += clusters[i]->getCenter().distanceTo(clusters[clust2]->getCenter());
        }
        
    }
    return score;
}
