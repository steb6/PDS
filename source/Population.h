#include "dependencies.h"

class Population{
    private:
        int pop_size = 0;
        int n_nodes = 0;
	MyRandom &myrandom;
	std::vector<int> mutation(std::vector<int>, double, City city);

    public:
        std::vector<std::vector<int>> population;
        std::vector<double> affinities;

	Population(int, int, MyRandom&);

	void generate_population();

	void calculate_affinities(City);

	std::vector<int> crossover(int, int, double, City, double);
	int pick_candidate();
};
