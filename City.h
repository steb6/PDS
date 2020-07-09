#include "dependencies.h"

class City{

    private:
        int n_nodes = 0;
        int x_gap = 0;
        int y_gap = 0;
        int border = 0;
	int top_bar = 0;

    public:
	std::vector<int> x;
        std::vector<int> y;
        City(int, int, int, int, int);
        void generate_graph();
	void generate_graph_thread(int);
        int path_length(std::vector<int>);
        int node_distance(int, int);
};
