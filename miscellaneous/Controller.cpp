//211 - Kikteva Veronika
//Zadacha 1

#include "Controller.hpp"

#include "../algorithms/algorithmsControl.h"
#include <cstring>
#include <signal.h>
#include <typeinfo>
#include <fstream>
#include <unistd.h>
#include <map>
#include "StringIs.h"
//singleton traits
Controller * Controller::instance = new Controller();

Controller & Controller::getInstance(){
    return *Controller::instance;
}


//listen a file that we read commands in
void Controller::acquireListener(FILE * fd){
    char command [1024];
    fprintf(stdout, "Listening file descriptor %d\n>>>", fileno(fd));
    while(fgets(command, 1024, fd)){
        processCommand(command);
        fprintf(stdout, "\n>>>");
    }
}

//function for svd vectors printing into file
static void printVector(FILE* file, const typename SVDProcessor::vector & v){
    fprintf(file, "%lf %lf %lf %lf\n", v.first.getX(), v.first.getY(), v.second.getX(), v.second.getY());
}

//load results of former find algorithms work
static std::vector<std::vector<Cluster>> loadResults(FILE * fin){
    std::map< int, std::map<int, std::vector<Point>> > inParse;
    double x, y;
    int color, ResIdx;
    char rdbuf [4000];
    while(fgets(rdbuf, 4000, fin)){
        sscanf(rdbuf, "%lf %lf %d %d", &x, &y, &color, &ResIdx);
        inParse[ ResIdx ][ color ].push_back(Point(x, y));
    }
    std::vector< std::vector<Cluster> > results;
    //process mapping
    for( auto algoKey: inParse ){
        std::vector<Cluster> localRes;
        for(auto clust: algoKey.second){
            Cluster cl;
            for( auto pointReference: clust.second ){
                cl.addPoint(pointReference);
            }
            localRes.push_back(cl);
        }
        results.push_back(localRes);
    }
    return results;
}

// get the point object from canvas underlying
std::vector<Point> Controller::extracted() {
    std::vector<Point> accumulated;
    for(auto& figure: canvas.getChildren()){
        if(1){
            Point copy (*static_cast<Point*>(figure));
            accumulated.push_back(copy);
        }
    }
    return accumulated;
}


//parse && process received command

void Controller::processCommand(const char * command){
    //razdelyaem stroku s komandoy na token i argument
    //strdup copiruet stroku, kotory my peredali chtoby mi mogli ee izmenyat'
    char * cmdcopy = strdup(command);//kopiruem, potomu chto cmdtok stroku razrezhet
    char * cmdtok = strtok(cmdcopy, " \n");
    //strtok zamenyaet na '\0' pervyi vstretivshiysa symvol
    //0 - idem po toy zhe stroke, prodolzhaem
    char * arg = strtok(0, "\n");
    if(strcmp(cmdtok, "DISP")==0){
        sscanf(arg, "%d", &disp);
        fprintf(stdout, "DISP set to %d successfully", disp);
        return;
    }
    if(strcmp(cmdtok, "GEN")==0){
        if(disp == 0){
            //my schitaem chto dispersiya ne ustanovlena i trebuem snachala ee ustanovit'
            fprintf(stderr, "Disp is not set. Set it by enter DISP [d:number]\n");
            return;
        }
        srand((unsigned)time(0));
        // we need disp to be set
        canvas.reset();
        //generiruem tochki
        for(int i = 0; i < 1000; ++i){
            double y_val = 0;
            //y - koordinata - normalnoe raspredelenie
            for(int iterGen = 0; iterGen < disp; ++iterGen){
                y_val += rand() % canvas.getHeight();
            }
            y_val /= disp;
            canvas.createPoint(
                               //po x - ravnomernoe
                rand() % canvas.getWidth(),
                y_val
            );
        }
        fprintf(stdout, "Generated dots successfully");
        return;
    }
    if(strcmp(cmdtok, "DRAW") == 0){
        canvas.draw();
        fprintf(stdout, "Draw performed to file \"%p.txt\" successfully", &canvas);
    }
    if(strcmp(cmdtok, "GIST")==0){
        canvas.createGist();
        fprintf(stdout, "Gist performed to file \"gist.txt\" successfully");
    }
    
    if(strcmp(cmdtok, "CLUST_GEN") == 0){
        int bounds[4];
        int disp;
        int amount;
        int color;
        sscanf(arg, "%d%d%d%d%d%d%d", bounds, bounds + 1, bounds + 2, bounds + 3, &disp, &amount, &color);
        Group g(disp, amount, bounds);
        std::vector<Point> points = g.generate();
        Cluster * cl = new Cluster();
        for(auto point:points){
            cl->addPoint(point);
        }
        canvas.addFigure(cl);
        printf("GROUP with id = %p generated. use id to rotate", (void*)cl);
        return;
    }
    
    if(strcmp(cmdtok, "ROTATE")==0){
        void * id;
        double angle;
        sscanf(arg, "%p%lf", &id, &angle);
        Cluster * c = dynamic_cast<Cluster*>(canvas.getById(id));
        if(c){
            c->rotate(angle);
            fprintf(stdout, "rotated succesfully");
        } else {
            fprintf(stdout, "please specify another id");
        }
    }
    
    
    
    if(strncmp("FIND:", cmdtok, 5)==0){
        char * type_token = strchr(cmdtok, ':')+1;
        processAlgorithm(type_token, arg);
        return;
    }
    if(strcmp(cmdtok, "ARCH")==0){
        std::vector<Point *> accumulated;
        for(auto& figure: canvas.getChildren()){
            if(figure->type == 1){
                Point * copy = new Point(*static_cast<Point*>(figure));
                accumulated.push_back(copy);
            } else if(figure->type == 2){
                Cluster * cl = dynamic_cast<Cluster*>(figure);
                for(auto& p: cl->getState()){
                    accumulated.push_back(&p);
                }
            }
        }
        FILE * f = fopen("__arch1", "w");
        for(auto point:accumulated){
            fprintf(f, "%lf %lf %d\n", point->getX(), point->getY(), 0);
        }
        fclose(f);
        return;
    }
    
    if(strcmp("EXTRACT", cmdtok)==0){
        char * __;
        long argN = strtol(arg, &__, 10);
        if( argN > results.size() - 1 ){
            printf("Cannot extract value excess of the bounds %lu\n", results.size() - 1);
            return;
        }
        auto fout = archieve_found(results[argN]);
        printf("success. result in file %s\n", fout.c_str());
    }
    
    if(cmdtok is "SAVE"){
        FILE * f = fopen("__farch", "w");
        for(int resIdx = 0; resIdx < results.size(); ++resIdx){
            FindResults res = results[resIdx];
            for(int clusterIdx = 0; clusterIdx < res.size(); ++clusterIdx){
                Cluster curCluster = res[clusterIdx];
                for( Point point: curCluster.getState() ){
                    fprintf(f, "%lf %lf %d %d\n", point.getX(), point.getY(), clusterIdx, resIdx);
                }
            }
        }
        printf("saved\n");
        fclose(f);
    }
    
    if(cmdtok is "LOAD"){
        FILE * fin = fopen("__farch", "r");
        if( not fin ){
            printf("nothing to load\n");
            return;
        }
        results = loadResults(fin);
        fclose(fin);
        return;
    }
    
    if(strcmp("SVD", cmdtok)==0){
        char fname [200];
        sprintf(fname, "__arch%s", arg);
        FILE * f = fopen(fname, "r");
        if(! f){
            printf("Not found file with clustering: %s. try again\n", fname);
            return;
        }
        std::map<int, std::vector<Point>> clusters;
        char rdbuf [1000];
        int color;
        double x, y;
        while(fgets(rdbuf, 1000, f)){
            sscanf(rdbuf, "%lf%lf%d", &x, &y, &color);
            if( clusters.count(color) == 0 ){
                clusters[color] = std::vector<Point>();
            }
            clusters[color].push_back(Point(x, y));
        }
        char vname [200];
        sprintf(vname, "vectors%ld", time(0) % 8377);
        FILE * vectorF = fopen(vname, "w");
        for(auto it:clusters){
            if(it.second.size() > 10){
                auto vectors = SVDProcessor().svdDecompose(it.second);
                printVector(vectorF, vectors.first);
                printVector(vectorF, vectors.second);
            }
        }
        fclose(vectorF);
        fclose(f);
        printf("svd decomposition finished. you might do the following with gnuplot:"
               "\"plot '%s' using 1:2:($3) with points lc rgb variable, '%s' using 1:2:3:4 with vectors filled head lw 3\"", fname ,vname);
        return;
    }
    
    
    if(strcmp(cmdtok, "DEARCH")==0){
        std::string argstr(arg);
        std::vector<Cluster *> clusters;
        std::ifstream ifs("__arch"+argstr);
        std::string point_repr;
        while(getline(ifs, point_repr)){
            char pbuf [1024];
            strcpy(pbuf, point_repr.c_str());
            double x, y;
            int color;
            sscanf(pbuf, "%lf %lf %d", &x, &y, &color);
            auto iter = std::find_if( clusters.begin(), clusters.end(), [&](Cluster * c)->bool{
                return c->getColor() == color;
            });
            Cluster * res;
            if(iter == clusters.end()){
                clusters.push_back(new Cluster());
                res = clusters.back();
                res->setColor(color);
            } else {
                res = *iter;
            }
            res->addPoint(* new Point(x, y) );
            
        }
        std::for_each( clusters.begin(), clusters.end(), [&](Cluster * c){
            printf("loaded %d points\n", c->getState().size());
            for(auto point : (*c).getState()){
                canvas.addFigure(new Point(point));
            }
        });
        return;
    }
    
    free(cmdcopy);
}
