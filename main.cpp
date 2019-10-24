//211 - Kikteva Veronika
//Zadacha 1

#include "Controller.hpp"
#include "ClusterBuilder.h"

int main(int argc, char ** argv) {
    //zapuskaem upravlyauschiy klass
    FILE * f = stdin;
    if(argc > 1)
        f = fopen(argv[1], "r");
    Controller::getInstance()
        .acquireListener(f);//schityvaem komandy s konsoli
}
