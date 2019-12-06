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
#include <ctime>
#include <map>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "../algorithms/algorithmsControl.h"
// u nas vsego odin object, poetomu mi sdelali constuctor privatnym
static std::string archieve_found(const std::vector<Cluster> &found, std::string name = "") {
    std::string time_arg = name + std::to_string(clock() % 9837);
    forEach(found, [&](Cluster &c){
        c.setColor(rand()%(1<<24));
        c.archieve(time_arg);
    });
    return time_arg;
}
class Controller{
private:
    //sdelali constructor privatnym, chtoby ne sozdavat exemplyary vne klassa
    
    //exemplyar, ediniza
    static Controller * instance;
    std::vector<Point> extracted();
    
    void processCommand(const char *);
    std::map<std::string, Algorithm*> commands;
    
    void addAlgorithm(std::string name, Algorithm* algorithm){
        commands[name] = algorithm;
    }
    
    FILE * logFile;
    
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
        std::string fout = archieve_found(found, name);
        fprintf(stdout, "Found. You might want to do the following: %s with %s", algorithm->tip, fout.c_str());
        fprintf(logFile, "%ld I algorithm %s worked\n", time(0), name);
    }
    
    Canvas canvas;
    int disp = 0;
public:
    //staticheskiy metod imeet dostup k privatnym polyam klassa i pozvolyaet rabotat' s exemplyarami, no v nego ne peredaets'a this
    static Controller & getInstance();
    //sadimsya na fail (stdin v osnovnom) i chitaem comandy
    void acquireListener(FILE*);
};


#endif /* Controller_hpp */
