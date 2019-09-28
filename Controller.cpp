//211 - Kikteva Veornika
//Zadacha 1

#include "Controller.hpp"
#include "cluster.hpp"
#include <cstring>

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
        srand(time(0));
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
        char color [100];
        sscanf(arg, "%d%d%d%d%d%d%s", bounds, bounds + 1, bounds + 2, bounds + 3, &disp, &amount, color);
        Cluster * c = &Cluster::Builder()
            .setPointAmount(amount)
            .setBounds(bounds)
            .setDisp(disp)
            .build();
        canvas.addFigure(c);
        c->setColor(color);
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
    
    free(cmdcopy);
}
