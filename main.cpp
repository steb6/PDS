/*
 * Genetic Algorithm for TSP SPM 2020
 *
 * Dipartimento di Informatica Università di Pisa
 *
 * Author: Stefano Berti
 *
 * Professors: Danelutto, Torquati
 *
 * Description: this is the project implementation
 *
 */

//TODO aggiungere costo relativo tempo overhead
//TODO prendere i tempi ed usarli in modo furbo con rplsh
//TODO provare vtune-gui

#include "dependencies.h"

#define RADIUS 10
#define X_SIZE 640
#define Y_SIZE 480
#define RESISTENCE 0.9 // probability that mutate does not happen
#define TOP_BAR 0 // height in pixel of information bar

using namespace std::chrono;

int main(int argc, char *argv[]){

	// ********************************* input reading /**********************************/
	int N_NODES=20;
	int POP_SIZE=1000;
	int NW=4; //TODO get core number
	int MODE=1;
	int ITERATIONS=3;

	// check if any arguments is given
	//if(argc > 1){
	    //wrong arguments number
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
	//}

	// ********************************* getting ready /**********************************/
	//std::cout << "Genetic algorithm for Traveling Salesman problem @Berti Stefano" << std::endl;
	City city(X_SIZE, Y_SIZE, RADIUS, N_NODES, TOP_BAR);
	city.generate_graph();
	#ifdef GRAPH
	Draw draw(RADIUS, N_NODES, TOP_BAR);
	#endif

	GA ga(city, RESISTENCE, NW, N_NODES, POP_SIZE, ITERATIONS);

	// ********************************* cycle /**********************************/
	 // timer
	long time;

	switch(MODE){
	    case 0: // sequential
		//std::cout << "Sequential version" << std::endl;
		#ifdef GRAPH
		    time = ga.evolution_seq(draw);
		#else
		    time = ga.evolution_seq();
		#endif
		break;
	    case 1: // thread
		//std::cout << "Thread version" << std::endl;
		#ifdef GRAPH
		    time = ga.evolution_thread(draw);
		#else
		    time = ga.evolution_thread();
		#endif
		break;
	    case 2: // fastflow
		//std::cout << "FastFlow version" << std::endl;
		#ifdef GRAPH
		    time = ga.evolution_ff(draw);
		#else
		    time = ga.evolution_ff();
		#endif
		break;
	}


	// ********************************* print time /**********************************/

	//std::cout.flush();
	//std::cout << std::endl;
	std::cout << "Microseconds for " << ITERATIONS << " loops: " << std::fixed << time << std::endl;

	#ifdef GRAPH
	draw.close();
	#endif
	return 0;
}
