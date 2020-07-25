#include "dependencies.h"

Population::Population(int p, int n, MyRandom& mr): myrandom(mr){
	pop_size = p;
	n_nodes = n;
	population = std::vector<std::vector<int>>(pop_size);
	affinities = std::vector<double>(pop_size);
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

void Population::calculate_affinities(City city){
    double sum = 0;
    for(int k=0; k<pop_size; k++){ // calculate score for every member of population
	double score = city.path_length(population[k]);
	affinities[k] = 1/(score+1); // invert score (shortest path are better), +1 to avoid crash
	sum += affinities[k];
    }
    for(int i=0; i<pop_size; i++) // VECTORIZED
	affinities[i] = affinities[i]/sum;
}
// ********************************* crossover and mutation /**********************************/

std::vector<int> Population::crossover(int a, int b, double resistence){
    if(a==b)
	return mutation(population[a], resistence); // same path

    int i = myrandom.get_random_node();
    int j = myrandom.get_random_node();

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

    int i = myrandom.get_random_node();
    int j = myrandom.get_random_node();

    double r = myrandom.get_real_number();
    if(r<resistence || i==j)
        return a;

    int aux = a[i];
    a[i] = a[j];
    a[j] = aux;
    return a;
}

int Population::pick_candidate(){

    double r = myrandom.get_real_number();
    int i=0;
    while(r>0){
	r -= affinities[i];
	i++;
    }
    i--;

    return i;
}
