
#include "spanTree.hpp"

double TreeFactory::vergeLength(TreeFactory::verge v){
    return v.first->distanceTo(*v.second);
}
