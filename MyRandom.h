#include "dependencies.h"

typedef std::mt19937 rng_type;

class MyRandom{
    public:
	int n_nodes;
	int pop_size;
	rng_type rng;
        std::uniform_int_distribution<rng_type::result_type> n;
        std::uniform_int_distribution<rng_type::result_type> p;
	std::uniform_real_distribution<> d;

	MyRandom(int, int);
	int get_random_node();
	int get_random_pop();
	double get_real_number();
};
