//211 - Kikteva Veronika
//Zadacha 1

#ifndef Controller_hpp
#define Controller_hpp

#include "../Objects/Canvas.h"
#include "../algorithms/kmeans.hpp"
#include "ClusterBuilder.h"
#include "../algorithms/Hierarchy.h"
#include "../algorithms/spanTree.hpp"
#include "../Objects/Group.hpp"
#include "../algorithms/vawe_algorithm.h"
#include "../Objects/cluster.hpp"
#include "../algorithms/dbscan.h"
#include "../algorithms/Forel.h"
#include "../algorithms/svd.h"
#include <ctime>
#include <map>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "../algorithms/algorithmsControl.h"

//controller holds canvas & processes the command input
//in MVC exactly this -- controller
class Controller{
private:
    //results of last search
    typedef std::vector<Cluster> FindResults;
    std::vector< FindResults > results;
    // as controller is a singleton
    static Controller * instance;
    // extracted points
    std::vector<Point> extracted();
    
    void processCommand(const char *);
    //some find commands that are treated in very special way
    std::map<std::string, Algorithm*> commands;
    //register an algorithm
    // - algorithm must be a stateless (once worked, ready to new data) class derived from Algorithm class and support such methods:
    //-> setup (for passing arguments)
    //-> find: (vector of points) -> vector of clusters
    void addAlgorithm(std::string name, Algorithm* algorithm){
        commands[name] = algorithm;
    }
    //logging actions. mainly algorithms
    FILE * logFile;
    
    //add known algos
    Controller(){
        addAlgorithm("VAWE",    new clusterFinder());
        addAlgorithm("KMEANS",  new kmeansFinder());
        addAlgorithm("SPAN",    new TreeFactory());
        addAlgorithm("HIERARCH",new hierchAlgorithm());
        addAlgorithm("DBSCAN",  new DBSCAN());
        addAlgorithm("FOREL",   new ForelAlgorithm());
        logFile = fopen("Geomwork.log", "a");
    }
    //args are real arguments that will be passed
    void processAlgorithm(char * name, char * args){
        auto finder = commands.find(name);
        if(finder == commands.end()){
            fprintf(stdout, "no such algorithm: %s, try again\n", name);
            fprintf(logFile, "%ld E attempt to access %s algorithm\n", time(0), name);
        }
        Algorithm * algorithm = finder->second;
        algorithm->setup(args);
        fprintf(logFile, "%ld I algorithm %s started working\n", time(0), name);
        std::vector<Cluster> found = algorithm->find(extracted());
        results.push_back(found);
        printf("pushed results of %s algo into %lu case\n", name, results.size() - 1);
        fprintf(logFile, "%ld I algorithm %s worked\n", time(0), name);
    }
    
    Canvas canvas;
    //some legacy when there was no ClusterBuilder
    int disp = 0;
public:
    
    static Controller & getInstance();
    
    void acquireListener(FILE*);
};


#endif /* Controller_hpp */
