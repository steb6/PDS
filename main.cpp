#include <unistd.h> //usleep

#include "City.h"
#include "Draw.h"
#include "Population.h"
#include "utilities.h"

#define N_NODES 10
#define POP_SIZE 50
#define RADIUS 10
#define X_SIZE 640
#define Y_SIZE 480
#define RESISTENCE 0.9 // probability that mutate does not happen
#define TOP_BAR 50 // height in pixel of information bar

#define GRAPH

// goodtest: 12 nodes, 50 populations, 10000 time
// goodtest: 20 nodes, 100 populations, 10000 time

int main(){

	#ifdef GRAPH
	Draw draw(RADIUS, N_NODES);
	#endif
	City city(X_SIZE, Y_SIZE, RADIUS, N_NODES);
	Population population(POP_SIZE, N_NODES);
	
	int i;
	i=0;

	city.generate_graph();
	population.generate_population();

	while(!kbhit()){
	    population.calculate_affinities(city);

	    #ifdef GRAPH
	    draw.clear();
	    draw.print_city(city.x, city.y);
	    draw.print_best_one(population.best_one, city.x, city.y);
	    draw.print_info(i, POP_SIZE, population.min_length);
	    #endif

	    log(i, population.min_length);
	    population.reproduce(RESISTENCE);
	    usleep(100000); // 1/10 of second
	    i++;
	}
	#ifdef GRAPH
	draw.close();
	#endif
	return 0;
}
