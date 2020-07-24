#include "dependencies.h"

MyRandom::MyRandom(int n, int p){

    n_nodes = n;
    pop_size = p;
    std::uniform_real_distribution<> d(0.0, 1.0);

}

int MyRandom::get_random_node(){

std::uniform_int_distribution<rng_type::result_type> udist(0, n_nodes);

  // seed rng first: std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).count();
  rng_type::result_type const seedval = time(NULL); // TODO deve cambiare ogni microsecondo!
  rng.seed(seedval);

  rng_type::result_type random_number = udist(rng);

  return random_number;
}

int MyRandom::get_random_pop(){
std::uniform_int_distribution<rng_type::result_type> udist(0, pop_size);

  // seed rng first: std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).count();
  rng_type::result_type const seedval = time(NULL); // get this from somewhere
  rng.seed(seedval);

  rng_type::result_type random_number = udist(rng);

  return random_number;
}

double MyRandom::get_real_number(){
    return d(rng);
}
