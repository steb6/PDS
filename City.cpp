#include "City.h"
#include <cstdlib> // rand
#include <cmath>  // sqrt
#include <math.h> // pow
#include <time.h> // time

City::City(int gap_x, int gap_y, int b, int n){
    x_gap = gap_x;
    y_gap = gap_y;
    border = b;
    n_nodes = n;
    x = std::vector<int>(n);
    y = std::vector<int>(n);
}

void City::generate_graph(){ // generate points, having gaps and border to avoid, y doesn't spawn in first 2*border pixels
    srand(time(NULL));
    for(int i=0; i<n_nodes; i++){
        x[i] = (rand() % (x_gap-border*2)) + border;
        y[i] = (rand() % (y_gap-border-border*2)) + border + border; // y
     }
}

int City::path_length(std::vector<int> path){
    int sum = 0;
    for(int i=0; i<n_nodes-1; i++){ // sum every distance of path
	sum += node_distance(path[i], path[i+1]);
    }
    return sum;
}

int City::node_distance(int a, int b){
    return sqrt(pow((x[a] - x[b]), 2) + pow((y[a] - y[b]), 2));
}

