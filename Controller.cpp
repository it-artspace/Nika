//211 - Kikteva Veornika
//Zadacha 1

#include "Controller.hpp"
#include "vawe_algorithm.h"
#include "cluster.hpp"
#include <cstring>
#include <signal.h>
#include <typeinfo>
#include <fstream>
#include <unistd.h>

//podgotavliveam controller k rabote
Controller * Controller::instance = new Controller();

Controller & Controller::getInstance(){
    return *Controller::instance;
}

Controller::Controller() :canvas(){}

//kazhday stroka - komanda
void Controller::acquireListener(FILE * fd){
    char command [1024];
    fprintf(stdout, "Listening file descriptor %d\n>>>", fd->_file);
    while(fgets(command, 1024, fd)){
        processCommand(command);
        fprintf(stdout, "\n>>>");
    }
}


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
        Cluster * c = &Cluster::Builder()
            .setPointAmount(amount)
            .setBounds(bounds)
            .setDisp(disp)
            .build();
        canvas.addFigure(c);
        c->setColor(color);
        std::for_each(c->getState().begin(), c->getState().end(), [&](const Point * c){
            IGeom * g = const_cast<Point*>(c);
            canvas.addFigure(g);
        });
        c->archieve();
        fprintf(stdout, "cluster with id = %p generated succesfully", (void*)c);
    }
    
    if(strcmp(cmdtok, "ROTATE")==0){
        void * id;
        double angle;
        sscanf(arg, "%p%lf", &id, &angle);
        Cluster * c = static_cast<Cluster*>(canvas.getById(id));
        if(c){
            c->rotate(angle);
            fprintf(stdout, "rotated succesfully");
        } else {
            fprintf(stdout, "please specify another id");
        }
    }
    if(strcmp(cmdtok, "FIND_V")==0){
        std::vector<Point *> accumulated;
        double treshold = strtod(arg, 0);
        for(auto& figure: canvas.getChildren()){
            if(figure->type == 1){
                Point * copy = new Point(*static_cast<Point*>(figure));
                accumulated.push_back(copy);
            }
        }
        for(auto elem: clusterFinder::vaweSearch(accumulated, treshold)){
            
            auto cluster_copy = elem;
            printf("found cluster and recorded to file %p.txt\n", (void*)cluster_copy);
            
            elem->setColor(rand() % (1<<24));
            elem->archieve();
        }
        return;
    }
    if(strcmp(cmdtok, "ARCH")==0){
        std::vector<Point *> accumulated;
        for(auto& figure: canvas.getChildren()){
            if(figure->type == 1){
                Point * copy = new Point(*static_cast<Point*>(figure));
                accumulated.push_back(copy);
            }
        }
        FILE * f = fopen("__arch", "w");
        for(auto point:accumulated){
            fprintf(f, "%lf %lf %d\n", point->getX(), point->getY(), 0);
        }
        fclose(f);
        return;
    }
    if(strcmp(cmdtok, "FIND_K")==0){
        std::vector<Point *> accumulated;
        for(auto& figure: canvas.getChildren()){
            if(figure->type == 1){
                Point * copy = new Point(*static_cast<Point*>(figure));
                accumulated.push_back(copy);
            }
        }
        remove("__arch");
        /*
         iterate over k, finding out minimum value
         *//*
        kmeansFinder::Iterator min = std::min_element(
            kmeansFinder::startFind( accumulated ),
            kmeansFinder::Iterator( (int)accumulated.size(), std::vector<Point*>()),
            [](double d1, double d2){
                return d1 < d2;
            }
        )*/
        kmeansFinder::Iterator min(1, accumulated);
        kmeansFinder::Iterator least = min;
        while(*least <= *min){
            least++;
        }
        min = least;
        std::vector<Cluster *>found = kmeansFinder(min.getK()).find(accumulated);
        std::for_each( found.begin(), found.end(), [&](Cluster * c)->void{
            c->setColor(rand() % (1<<24));
            c->archieve();
            printf("found cluster with %d points\n", c->getState().size());
        });
        
        printf("k = %d, value = %lf, %d clusters acrhieved\n", min.getK(), *min, found.size());
        return;
    }
    if(strcmp(cmdtok, "DEARCH")==0){
        std::vector<Cluster *> clusters;
        std::ifstream ifs("__arch");
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
            for(auto point : (*c).getState()){
                IGeom * g = const_cast<Point*>(point);
                canvas.addFigure(g);
            }
        });
        return;
    }
    
    free(cmdcopy);
}
