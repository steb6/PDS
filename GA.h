#include "dependencies.h"

class GA{
    public:
	City &city;
	double resistence;
	int nw;
	int n_nodes;
	int pop_size;
	int iterations;
 
        GA(City&, double, int, int, int, int);

        void evolution_seq();
        void evolution_thread();
        void evolution_ff();
};
