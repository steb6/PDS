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

	#ifdef GRAPH
            long evolution_seq(Draw);
            long evolution_thread(Draw);
            long evolution_ff(Draw);
	#else
	    long evolution_seq();
            long evolution_thread();
            long evolution_ff();
	#endif


};
