
#include "spanTree.hpp"

double TreeFactory::vergeLength(const TreeFactory::verge &v){
    return (*v.first)->distanceTo(**v.second);
}
