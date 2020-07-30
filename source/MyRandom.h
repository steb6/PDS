#include "dependencies.h"

typedef std::mt19937 rng_type;

class MyRandom{
    private:
	rng_type rng;
        std::uniform_int_distribution<rng_type::result_type> ndist;
        std::uniform_int_distribution<rng_type::result_type> pdist;
	std::uniform_real_distribution<> d;

    public:
	MyRandom(int, int);
	int get_random_node();
	int get_random_pop();
	double get_real_number();
};
