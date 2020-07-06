#include "City.h"
#include "Population.h"
#include "utilities.h"

#include <algorithm>
#include <cfloat>
#include <set>
#include <atomic>
#include <thread>
#include <iostream>
#include <ff/parallel_for.hpp>
#include <mutex>

using namespace ff;

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
	    for(int i=0; i<n_nodes; i++) // VECTORIZED
		path[i] = i;
	    std::random_shuffle(path.begin(), path.end());
	    population[k] = path;
    }
}

// ********************************* calculate_affinities /**********************************/

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
    for(int i=0; i<pop_size; i++) // VECTORIZED
	affinities[i] = affinities[i]/sum;
}

void Population::calculate_affinities_thread(City city, int nw){

    std::mutex mtx;
    std::atomic<double> sum{0};
    std::vector<std::thread> threads;

    int chunk_size = pop_size/nw;

    // define threads behaviour
    auto myJob = [this, chunk_size, &city, &sum, &mtx](int k) {
	double score;
        for(int i=k*chunk_size; i<(k+1)*chunk_size; i++){
            score = city.path_length(population[i]);
	    if(score<min_length){
		mtx.lock();
		min_length = score;
		best_one = population[i];
		mtx.unlock();
	    }
	    affinities[i] = 1/(score+1);
	    sum = sum + affinities[i];
	}
    };

    // start threads
    for (int i=0; i<nw; i++)
        threads.push_back(std::thread(myJob, i));
    for (int i=0; i<nw; i++)
        threads[i].join();
    // normalize
    for(int i=0; i<pop_size; i++)
	affinities[i] = affinities[i]/sum;
}

void Population::calculate_affinities_ff(City city, int nw){
    double sum=0;
    ParallelForReduce<double> pfr(nw);
    pfr.parallel_reduce(sum, 0, //reduction variable, identity-value
			0, population.size(), //first, last
			1, 0, //step, chunksize
			[&city, this](const long i, double &mysum){ //mapF
			    double score = city.path_length(population[i]);
			    if(score < min_length){ // TODO could cause problem, understand how to do it
				min_length = score;
				best_one = population[i];
			    }
			    score = 1/(score+1);
			    affinities[i] = score;	    
			    mysum += score;
			},
			[](double &s, const double e){ s+= e; }, //reduceF
			nw); // number of threads, we have 5

    // normalization
    ParallelFor pf(nw);
    pf.parallel_for_idx(0, pop_size,
			1, 0, //step, chunksize
			[this, sum](const long begin, const long end, const long thid)  {
			    for(long i=begin; i<end; ++i){ //TODO VECTORIZED but possible aliasing
                                affinities[i] = affinities[i]/sum;
			    }
			});
}

// ********************************* reproduce /**********************************/

void Population::reproduce_all(double resistence){	
    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size);
    for(int i=0; i<pop_size; i++)
	newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);

    // evolve
    population = newborn;
}

void Population::reproduce_all_ff(City city, double resistence, int nw){

    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size);
    std::vector<double> new_affinities = std::vector<double>(pop_size);
    double sum = 0;

    ParallelForReduce<double> pfr(nw);
    pfr.parallel_reduce(sum, 0, //reduction variable, identity-value
			0, population.size(), //first, last
			1, 0, //step, chunksize
			[this, &city, &newborn, &new_affinities, resistence](const long i, double &mysum){ //mapF
			    newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);
			    double score = city.path_length(population[i]);
			    if(score < min_length){ // TODO could cause problem, understand how to do it
				min_length = score;
				best_one = population[i];
			    }
			    score = 1/(score+1);
			    new_affinities[i] = score;	    
			    mysum += score;
			},
			[](double &s, const double e){ s+= e; }, //reduceF
			nw); // number of threads, we have 5

    // normalization
    ParallelFor pf(nw);
    pf.parallel_for_idx(0, pop_size,
			1, 0, //step, chunksize
			[this, sum, &new_affinities](const long begin, const long end, const long thid)  {
			    for(long i=begin; i<end; ++i){// TODO VECTORIZED but versioned
                                new_affinities[i] = new_affinities[i]/sum;
			    }
			});
    // evolve
    affinities = new_affinities;
    population = newborn;
}

void Population::reproduce_all_thread(City city, double resistence, int nw){

    std::vector<std::thread> threads;

    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size);
    std::vector<double> new_affinities = std::vector<double>(pop_size);

    int chunk_size = pop_size/nw;
    std::mutex mtx;
    std::atomic<double> sum{0};

    // define threads behaviour
    auto myJob = [this, &mtx, &sum, &city, chunk_size, resistence, &newborn, &new_affinities](int k) {
        for(int i=k*chunk_size; i<(k+1)*chunk_size; i++){
            newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);
	    double score = city.path_length(newborn[i]);
	    if(score<min_length){
	        mtx.lock();
	        min_length = score;
	        best_one = newborn[i];
	        mtx.unlock();
	    }
	    new_affinities[i] = 1/(score+1);
	    sum = sum + new_affinities[i];
	}

    };

    // start threads
    for (int i=0; i<nw; i++)
        threads.push_back(std::thread(myJob, i));
    for (int i=0; i<nw; i++)
        threads[i].join();

    // normalize
    for(int i=0; i<pop_size; i++)
	new_affinities[i] = new_affinities[i]/sum;

    // evolve
    population = newborn;
    affinities = new_affinities;
}



// ********************************* crossover and mutation /**********************************/

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
	for(; h<n_nodes; h++) // if mom[h] is not in removed, i don't need to see again mom[h]
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
