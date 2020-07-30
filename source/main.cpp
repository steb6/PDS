/*
 * Genetic Algorithm for TSP SPM 2020
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Stefano Berti
 *
 * Professors: Danelutto, Torquati
 *
 * Description: parallel genetic algorithm for traveling salesman problem resolution
 *
 * Example of usage: ./demo 1 20 10000 4 100 
 *
 */

#include "dependencies.h"

#define CONNECTION_PROB 0.1
#define RADIUS 10 // radius of nodes in graphical version
#define X_SIZE 640
#define Y_SIZE 480
#define RESISTENCE 0.9 // probability that a mutation DOES NOT happen
#define CROSSOVER_PROB 0.9 // probability that a crossover DOES happen

using namespace std::chrono;

int main(int argc, char *argv[]){

        srand(5);

	// ********************************* input reading /**********************************/
	int N_NODES=0;
	int POP_SIZE=0;
	int NW=0;
	int MODE=0;
	int ITERATIONS=0;


 	if (argc != 6) {
    	    errno = EINVAL;
    	    perror("Usage: {0:SEQ 1:THREAD 2:FF} N_NODES POP_SIZE NW ITERATIONS\n");
    	    return -1;
	}
	// good arguments number, get them
	MODE = atoi(argv[1]);
	N_NODES = atoi(argv[2]);
	POP_SIZE = atoi(argv[3]);
	NW = atoi(argv[4]);
	ITERATIONS = atoi(argv[5]);
	// invalid atoi conversion gives us 0
	if(N_NODES == 0 || POP_SIZE == 0 || NW == 0 || MODE<0 || MODE>2 || ITERATIONS == 0){
    	    errno = EINVAL;
    	    perror("Usage: {0:SEQ 1:THREAD 2:FF} N_NODES POP_SIZE NW ITERATIONS\n");
    	    return -1;
	}

	// ********************************* getting ready /**********************************/
	City city(X_SIZE, Y_SIZE, RADIUS, N_NODES);
	city.generate_graph(CONNECTION_PROB);
	GA ga(city, RESISTENCE, NW, N_NODES, POP_SIZE, ITERATIONS, CROSSOVER_PROB);

	// ********************************* cycle /**********************************/
	 // timer
	long time;

	switch(MODE){
	    case 0:
		#ifdef GRAPH
		{
		    Draw draw(RADIUS, N_NODES);
		    time = ga.evolution_seq(draw);
		    std::cout << "This is the best solution found, press any key to continue" << std::endl;
		    while(!kbhit());
		    draw.close();
		}
		#else
		    time = ga.evolution_seq();
		#endif
		break;
	    case 1:
		time = ga.evolution_thread();
		break;
	    case 2:
		time = ga.evolution_ff();
		break;
	}

	// ********************************* print time /**********************************/
	std::cout << "Microseconds for " << ITERATIONS << " loops: " << std::fixed << time << std::endl;
	return 0;
}
