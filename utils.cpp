
#include "utils.hpp"

//map function
template<typename Result_t, typename Basic_t, typename Transformer_t>
std::vector<Result_t> remapVector(Basic_t source, Transformer_t remapper){
    std::vector<Result_t> res;
    for(auto element: source){
        res.push_back(remapper(element));
    }
    return res;
}


//applies function to all collection elements. They can be mutated and even reassigned
template<typename Basic_t, typename Transformer_t>
void forEach(Basic_t source, Transformer_t remapper){
    for(auto element: source){
        remapper(element);
    }
}
