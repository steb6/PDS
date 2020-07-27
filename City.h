#include "dependencies.h"

class City{

    private:
        int n_nodes = 0;
        int x_gap = 0;
        int y_gap = 0;
        int border = 0;
        int node_distance(int, int);
	bool adj_is_connected_graph();

    public:
	std::vector<std::vector<bool>> adj;
	std::vector<int> x;
        std::vector<int> y;
        City(int, int, int, int);
        void generate_graph(double);
        int path_length(std::vector<int>);
	bool is_legal_path(std::vector<int>);
};
