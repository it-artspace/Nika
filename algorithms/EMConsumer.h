

#ifndef EMConsumer_h
#define EMConsumer_h
#include <vector>
#include "../Objects/cluster.hpp"

const static char * template_ = "set terminal gif animate delay 100\n"
"set output 'foobar.gif'\n"
"stats 'datafile' nooutput\n"
"set xrange [-1000:1000]\n"
"set yrange [-1000:1000]\n"
"do for [i=1:int(STATS_blocks)] {\n"
    "plot 'datafile' index (i-1) using 1:2:($3) with points lc rgb variable\n"
"}\n";


class EMConsumer{
    FILE * writer;
    void print_entry(const Cluster & cl, int no){
        fprintf(writer, "%lf %lf %d\n", cl[no].getX(), cl[no].getY(), cl.getColor());
    }
public:
    EMConsumer(){
        writer = fopen("datafile", "w");
    }
    
    
    void write_chunk(std::vector<Cluster> clusters){
        for( const auto& cl: clusters ){
            for(int i = 0; i < cl.size(); i++){
                print_entry(cl, i);
            }
        }
        fprintf(writer, "\n\n");
    }
    ~EMConsumer(){
        fclose(writer);
        FILE * f = fopen("plot.txt", "w");
        fprintf(f, "%s", template_);
        fclose(f);
    }
};

#endif /* EMConsumer_h */
