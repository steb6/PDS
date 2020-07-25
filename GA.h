#include "dependencies.h"

class GA{
    private:
	City &city;
	double resistence;
	int nw;
	int n_nodes;
	int pop_size;
	int iterations;

    public:
        GA(City&, double, int, int, int, int);

	#ifdef GRAPH
            long evolution_seq(Draw);
        #else
	    long evolution_seq();
	#endif

        long evolution_thread();
        long evolution_ff();

};
