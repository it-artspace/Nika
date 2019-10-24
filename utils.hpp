

#ifndef utils_hpp
#define utils_hpp

#include "point.h"
#include <vector>
template<typename Result_t, typename Basic_t, typename Transformer_t>
std::vector<Result_t> remapVector(Basic_t, Transformer_t);


template< typename Basic_t, typename Transformer_t>
void forEach(Basic_t, Transformer_t);

#endif /* utils_hpp */
