//211 - Kikteva Veronika
//Zadacha 1

#include "miscellaneous/Controller.hpp"
#include "miscellaneous/ClusterBuilder.h"
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char ** argv) {
    //zapuskaem upravlyauschiy klass
    FILE * f = stdin;
    if(argc > 1)
        f = fopen(argv[1], "r");
    
    auto handler = [](int){
        void *callstack[128];
        int frames = backtrace(callstack, 128);
        char **strs= backtrace_symbols(callstack, frames);
        FILE *f = fopen("crash_report.txt", "w");
        if (f){
            for(int i = 0; i < frames; ++i){
                fprintf(f, "%s\n", strs[i]);
            }
            fclose(f);
        }
        free(strs);
    };
    signal(11, handler);
    signal(14, handler);
    Controller::getInstance()
        .acquireListener(f);//schityvaem komandy s konsoli
    
}
