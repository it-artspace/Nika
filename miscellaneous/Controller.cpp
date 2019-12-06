//211 - Kikteva Veronika
//Zadacha 1

#include "Controller.hpp"

#include "../algorithms/algorithmsControl.h"
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


//kazhday stroka - komanda
void Controller::acquireListener(FILE * fd){
    char command [1024];
    fprintf(stdout, "Listening file descriptor %d\n>>>", fileno(fd));
    while(fgets(command, 1024, fd)){
        processCommand(command);
        fprintf(stdout, "\n>>>");
    }
}


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
        std::for_each(points.begin(), points.end(), [&](Point c){
            canvas.addFigure(new Point(c));
        });
        return;
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
    
    
    
    if(strncmp("FIND:", cmdtok, 5)==0){
        char * type_token = strchr(cmdtok, ':')+1;
        processAlgorithm(type_token, arg);
        return;
    }
    if(strcmp(cmdtok, "ARCH")==0){
        std::vector<Point *> accumulated;
        for(auto& figure: canvas.getChildren()){
            if(1){
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
