#include "dependencies.h"

MyRandom::MyRandom(int n, int p){
    std::uniform_int_distribution<rng_type::result_type> ndistx(0, n-1);
    std::uniform_int_distribution<rng_type::result_type> pdistx(0, p-1);
    ndist = ndistx;
    pdist = pdistx;
    rng.seed(time(NULL));
}

int MyRandom::get_random_node(){
    rng_type::result_type random_number = ndist(rng);
    return random_number;
}

int MyRandom::get_random_pop(){
    rng_type::result_type random_number = pdist(rng);
    return random_number;
}

double MyRandom::get_real_number(){
    return d(rng);
}
