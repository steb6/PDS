#include "dependencies.h"

MyRandom::MyRandom(int n, int p){
    n_nodes = n;
    pop_size = p;
    std::uniform_int_distribution<rng_type::result_type> ndistx(0, n_nodes-1);
    std::uniform_int_distribution<rng_type::result_type> pdistx(0, pop_size-1);
    ndist = ndistx;
    pdist = pdistx;
}

int MyRandom::get_random_node(){

    rng_type::result_type const seedval = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    rng.seed(seedval);

    rng_type::result_type random_number = ndist(rng);

    return random_number;
}

int MyRandom::get_random_pop(){

    rng_type::result_type const seedval = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    rng.seed(seedval);

    rng_type::result_type random_number = pdist(rng);

    return random_number;
}

double MyRandom::get_real_number(){
    return d(rng);
}
