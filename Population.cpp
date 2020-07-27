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

std::vector<int> Population::crossover(int a, int b, double resistence, City city, double crossover_prob){
    if(a==b)
	return mutation(population[a], resistence, city); // same path

    double r = myrandom.get_real_number();
    if(r<crossover_prob)
        return mutation(population[a], resistence, city); // same path

    std::vector<int> dad = std::vector<int>(n_nodes);
    std::vector<int> mom = std::vector<int>(n_nodes);

    do{
        int i = myrandom.get_random_node();
        int j = myrandom.get_random_node();

        if(i==j)
            return mutation(population[a], resistence, city); // |list to change| = 0

        if(i>j){  // i must be < j
	    int aux = i;
            i = j;
	    j = aux;
        }

	for(int l=0; l<n_nodes; l++) dad[l] = population[a][l];
	for(int l=0; l<n_nodes; l++) mom[l] = population[b][l];

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
    }
    while(!city.is_legal_path(dad));

    return mutation(dad, resistence, city);
}

std::vector<int> Population::mutation(std::vector<int> a, double resistence, City city){

    double r = myrandom.get_real_number();
    if(r<resistence)
        return a;

    std::vector<int> candidate = std::vector<int>(n_nodes);

    do{

    for(int l=0; l<n_nodes; l++) candidate[l] = a[l];

    int i = myrandom.get_random_node();
    int j = myrandom.get_random_node();

    if(i==j)
	continue;

    int aux = candidate[i];
    candidate[i] = candidate[j];
    candidate[j] = aux;

    }
    while(city.is_legal_path(candidate));
    return candidate;
}

int Population::pick_candidate(){
    //return myrandom.get_random_pop();
    double r = myrandom.get_real_number();
    int i=0;
    while(r>0){
	r -= affinities[i];
	i++;
    }
    i--;

    return i;
}

