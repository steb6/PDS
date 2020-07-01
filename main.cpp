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
 *
 * Example of usage: ./demo 20 120 4
 *
 */

// compile:  g++ -Wall -g *.cpp -o demo -pthread -lgraph
// debug: gdb --args ./demo 4 4 4

// 0 = sequential, 1 = thread, 2 = fastflow

#define MODE 1
//#define GRAPH

#include <unistd.h> //usleep
#include <cstdlib> // atoi
#include <chrono>

#include "City.h"

#ifdef GRAPH
#include "Draw.h"
#endif

#include "Population.h"
#include "utilities.h"
#include <iostream>
#include <cfloat>

#define MAX_ITER 1000000
#define RADIUS 10
#define X_SIZE 640
#define Y_SIZE 480
#define RESISTENCE 0.9 // probability that mutate does not happen
#define TOP_BAR 0 // height in pixel of information bar




using namespace std::chrono;

int main(int argc, char *argv[]){

	 // timer
	high_resolution_clock::time_point program_start = high_resolution_clock::now();
	high_resolution_clock::time_point program_end;

	high_resolution_clock::time_point start;
	high_resolution_clock::time_point stop;

	int cycle_time=0;
	int program_time=0;

	// ********************************* input reading /**********************************/
	int N_NODES=15;
	int POP_SIZE=10000;
	int NW=5; //TODO get core number

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

	// ********************************* getting ready /**********************************/
	std::cout << "Genetic algorithm for Traveling Salesman problem @Berti Stefano" << std::endl;
	City city(X_SIZE, Y_SIZE, RADIUS, N_NODES, TOP_BAR);
	Population population(POP_SIZE, N_NODES);
	
	int i;
	i=0;

	start = high_resolution_clock::now();

	switch(MODE){
	    case 0: // sequential
		std::cout << "Sequential version" << std::endl;
		city.generate_graph();
		population.generate_population();
		break;
	    case 1: // thread
		std::cout << "Thread version" << std::endl;
		city.generate_graph();
		population.generate_population();
		break;
	    case 2: // fastflow
		std::cout << "FastFlow version" << std::endl;
		city.generate_graph();
		population.generate_population();
		break;
	}

	stop = high_resolution_clock::now();
	cycle_time = duration_cast<microseconds>(stop - start).count();
	std::cout << "Microseconds for initialization: " << cycle_time << std::endl;


	// ********************************* cycle /**********************************/
	#ifdef GRAPH
	Draw draw(RADIUS, N_NODES, TOP_BAR);
	double best = DBL_MAX;
	#endif
	// initialize affinities
	population.calculate_affinities(city);

	while(!kbhit() && i<MAX_ITER){

	    start = high_resolution_clock::now();

	    switch(MODE){
	        case 0: // sequential
		    population.calculate_affinities(city);
		    population.reproduce_all(RESISTENCE);
		    break;
	        case 1: // thread
		    population.reproduce_all_thread(city, RESISTENCE, NW);
		    break;
	        case 2: // fastflow
		    population.reproduce_all_ff(city, RESISTENCE, NW);
		    break;
	    }

	    #ifdef GRAPH
	    if(population.min_length < best){
	        draw.clear();
	        draw.print_city(city.x, city.y);
	        draw.print_best_one(population.best_one, city.x, city.y);
		best = population.min_length;
	    }
	    //draw.print_info(i, POP_SIZE, NW, program_time, N_NODES);
	    #endif

	    i++;

	    program_end = high_resolution_clock::now();
	    program_time = duration_cast<microseconds>(program_end - program_start).count();
	    
	    log(i, POP_SIZE, NW, program_time, N_NODES);
	    //std::cout << "Microseconds for loop: " << cycle_time << std::endl;
	}

	// ********************************* print time /**********************************/

	program_end = high_resolution_clock::now();
	program_time = duration_cast<microseconds>(program_end - program_start).count();
	std::cout.flush();
	std::cout << std::endl;
	std::cout << "Seconds for " << i << " loops: " << program_time/1000000. << std::endl;

	#ifdef GRAPH
	std::cout << "Press any key to quit" << std::endl;
	while(!kbhit());
	draw.close();
	#endif
	return 0;
}
