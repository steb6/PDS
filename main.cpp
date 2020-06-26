#include <unistd.h> //usleep
#include <cstdlib> // atoi
#include <chrono>

#include "City.h"
#include "Draw.h"
#include "Population.h"
#include "utilities.h"
#include <iostream>

#define MAX_ITER 1000
#define RADIUS 10
#define X_SIZE 640
#define Y_SIZE 480
#define RESISTENCE 0.9 // probability that mutate does not happen
#define TOP_BAR 50 // height in pixel of information bar

//#define GRAPH

using namespace std::chrono;

int main(int argc, char *argv[]){
	 // timer
	std::chrono::high_resolution_clock::time_point program_start;
	program_start = high_resolution_clock::now();

	/********************************** input reading /**********************************/
	int N_NODES=0;
	int POP_SIZE=0;
	int NW=0;
	// check if any arguments is given
	if(argc > 1){
	    //wrong arguments number
 	    if (argc != 4) {
    	        errno = EINVAL;
    	        perror("Usage: N_NODES POP_SIZE NW");
    	        return -1;
	    }
	    // good arguments number, get them
	    N_NODES = atoi(argv[1]);
	    POP_SIZE = atoi(argv[2]);
	    NW = atoi(argv[3]);
	    // invalid atoi conversion gives us 0
	    if(N_NODES == 0 || POP_SIZE == 0 || NW == 0){
    	        errno = EINVAL;
    	        perror("Usage: N_NODES POP_SIZE NW");
    	        return -1;
	    }
	}
	// use default values
	else{
	    N_NODES = 20;
	    POP_SIZE = 120;
	}

	/********************************** getting ready /**********************************/
	std::cout << "Genetic algorithm for Traveling Salesman problem" << std::endl;
	#ifdef GRAPH
	Draw draw(RADIUS, N_NODES);
	#endif
	City city(X_SIZE, Y_SIZE, RADIUS, N_NODES);
	Population population(POP_SIZE, N_NODES);
	
	int i;
	i=0;

	city.generate_graph();
	population.generate_population();

	high_resolution_clock::time_point start;
	high_resolution_clock::time_point stop;
	int cycle_time=0;

	/********************************** cycle /**********************************/
	while(!kbhit() && i<MAX_ITER){
	    start = high_resolution_clock::now();

	    population.calculate_affinities(city);

	    #ifdef GRAPH
	    draw.clear();
	    draw.print_city(city.x, city.y);
	    draw.print_best_one(population.best_one, city.x, city.y);
	    draw.print_info(i, POP_SIZE, population.min_length);
	    #endif

	    //population.reproduce(RESISTENCE);
	    population.reproduce_all(RESISTENCE);
	    //usleep(100000); // 1/10 of second
	    i++;

	    stop = high_resolution_clock::now();
	    cycle_time = duration_cast<microseconds>(stop - start).count();

	    log(i, population.min_length);
	    std::cout << "Microseconds for loop: " << cycle_time << std::endl;
	}

	/********************************** print time /**********************************/
	high_resolution_clock::time_point program_end;
	program_end = high_resolution_clock::now();

	int program_time = duration_cast<microseconds>(program_end - program_start).count();
	std::cout << "Microseconds for " << i << " loops: " << program_time << std::endl;

	#ifdef GRAPH
	draw.close();
	#endif
	return 0;
}
