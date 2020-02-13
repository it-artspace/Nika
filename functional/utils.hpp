//211 - Kikteva Veronika
//Zadacha 1


#ifndef utils_hpp
#define utils_hpp

#include <vector>
//standard implementations of high-order functions
template<typename Result_t, typename Basic_t, typename Transformer_t>
std::vector<Result_t> remapVector(Basic_t, Transformer_t);


template< typename Basic_t, typename Transformer_t>
void forEach(Basic_t, Transformer_t);
template<typename T, typename F, typename R>
T reduceVector(R vector, F reducer);
#endif /* utils_hpp */
