#include "City.h"
#include "Population.h"
#include "utilities.h"

#include <algorithm>
#include <cfloat>
#include <set>

Population::Population(int p, int n){
	pop_size = p;
	n_nodes = n;
	population = std::vector<std::vector<int>>(pop_size);
	affinities = std::vector<double>(pop_size);
	min_length = DBL_MAX;
}

void Population::generate_population(){
    for(int k=0; k<pop_size; k++){
	std::vector<int> path = std::vector<int>(n_nodes);
	    for(int i=0; i<n_nodes; i++)
		path[i] = i;
	    std::random_shuffle(path.begin(), path.end());
	    population[k] = path;
    }
}

void Population::calculate_affinities(City city){
    double sum = 0;
    for(int k=0; k<pop_size; k++){ // calculate score for every member of population
	double score = city.path_length(population[k]);
	if(score<min_length){
	    min_length = score; // index
	    best_one = population[k]; // path
	}
	affinities[k] = 1/(score+1); // invert score (shortest path are better), +1 to avoid crash
	sum += affinities[k];
    }
    for(int i=0; i<pop_size; i++)
	affinities[i] = affinities[i]/sum;
}

//version that keep half of previous generation
void Population::reproduce(double resistence){	
    // generate pop_size/2 new 
    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size/2);
    for(int i=0; i<pop_size/2; i++)
	newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);

    //find median affinities to filter previous path
    std::vector<double> affinities_sorted = affinities;
    std::sort(affinities_sorted.begin(), affinities_sorted.end());
    double median = affinities_sorted[pop_size/2];

    //add in newborn path of previous generation with affinity higher median untill we have pop_size paths
    for(int i=0; i<pop_size && newborn.size()<pop_size; i++)
	if(affinities[i] >= median)
	    newborn.push_back(population[i]);

    population = newborn;
}
 // version that replaces all the population
void Population::reproduce_all(double resistence){ //keep half of previous generation	
    // generate pop_size/2 new 
    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size);
    for(int i=0; i<pop_size; i++)
	newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);

    population = newborn;
}


std::vector<int> Population::crossover(int a, int b, double resistence){
    if(a==b)
	return mutation(population[a], resistence); // same path

    int i = rand()%n_nodes;
    int j = rand()%n_nodes;

    if(i==j)
        return mutation(population[a], resistence); // |list to change| = 0

    if(i>j){  // i must be < j
	int aux = i;
        i = j;
	j = aux;
    }

    std::vector<int> dad = population[a];
    std::vector<int> mom = population[b];

    std::set<int> removed; // elements of dad in [i, j] inclusive are placed in set
    for(int k=0; k<j-i+1; k++)
	removed.insert(dad[i+k]); // from dad[i+0] to dad[i+j-i] = dad[j] inclusive

    for(int k=0; k<j-i+1; k++){ // elements of dad in [i, j] inclusive must be replaced
	int h=0;
	for(h; h<n_nodes; h++) // if mom[h] is not in removed, i don't need to see again mom[h]
	    if(removed.find(mom[h])!=removed.end()){ // if h-th elements of mom is in removed
                dad[i+k] = mom[h];
		removed.erase(removed.find(mom[h]));
		break;
    	    }
    }

    return mutation(dad, resistence);
}

std::vector<int> Population::mutation(std::vector<int> a, double resistence){
    int i = rand()%n_nodes;
    int j = rand()%n_nodes;

    double r = ((double) rand() / (RAND_MAX));
    if(r<resistence || i==j)
        return a;

    int aux = a[i];
    a[i] = a[j];
    a[j] = aux;
    return a;
}
