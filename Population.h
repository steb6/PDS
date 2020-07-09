#include "dependencies.h"

class Population{
    private:
        int pop_size = 0;
        int n_nodes = 0;

    public:
        std::vector<std::vector<int>> population;
        std::vector<double> affinities;
	double min_length;
        std::vector<int> best_one;

	Population(int, int);

	void generate_population();
	void generate_population_thread(int);

	void calculate_affinities(City);

	std::vector<int> crossover(int, int, double);
	std::vector<int> mutation(std::vector<int>, double);

};
