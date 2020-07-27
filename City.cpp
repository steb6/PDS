#include "dependencies.h"

City::City(int gap_x, int gap_y, int b, int n){
    x_gap = gap_x;
    y_gap = gap_y;
    border = b;
    n_nodes = n;
    x = std::vector<int>(n);
    y = std::vector<int>(n);
    adj = std::vector<std::vector<bool>>(n, std::vector<bool>(n)); // optimized s.t. it occupies n*n bit
}

void City::generate_graph(double CONNECTION_PROB){ // generate points, having gaps and border to avoid, y doesn't spawn in first 2*border pixels
    for(int i=0; i<n_nodes; i++){
        x[i] = (rand() % (x_gap-border*2)) + border; // rand causes overhead, but here we use it few times
        y[i] = (rand() % (y_gap-border*2)) + border;
     }
    do{
	for(int i=0; i<n_nodes; i++){
	    for(int j=0; j<n_nodes; j++){
		double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		adj[i][j] = r < CONNECTION_PROB;
		adj[j][i] = r < CONNECTION_PROB;
	    }
	}	    
    }while(!adj_is_connected_graph());
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

bool City::adj_is_connected_graph(){ // at least 2 connection per city because we want all path of the same size

    std::vector<bool> visited = std::vector<bool>(n_nodes);
    for(int i=0; i<n_nodes; i++)
	visited[i] = false;

    std::vector<int> actual = std::vector<int>();
    actual.push_back(0);
    visited[0] = true;

    while(actual.size()>0){
	int node = actual.back();
	actual.pop_back();
	for(int i=0; i<n_nodes; i++){
	    if(adj[node][i] && !visited[i]){
		actual.push_back(i);
		visited[i] = true;
	    }
	}
    }

    bool connected = true;
    for(int i=0; i<n_nodes; i++)
	if(!visited[i])
	    connected = false;

    if(!connected)
	return connected;

    for(int i=0; i<n_nodes; i++){
	int fan=0;
	for(int j=0; j<n_nodes; j++){
	    if(adj[i][j])
		fan++;
	}
	if(fan<2)
	    return false;
    }

    return true;
}

bool City::is_legal_path(std::vector<int> path){
    for(int i=0; i<n_nodes-1; i++)
	if(!adj[path[i]][path[i+1]])
	    return false;
    return true;
}
