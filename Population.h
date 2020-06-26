#include <vector>

class Population{
    private:
        int pop_size = 0;
        int n_nodes = 0;
        std::vector<std::vector<int>> population;
        std::vector<double> affinities;


    public:
	double min_length;
        std::vector<int> best_one;
	Population(int, int);
	void generate_population();
	void reproduce(double);
	void reproduce_all(double);
	void calculate_affinities(City);
	std::vector<int> crossover(int, int, double);
	std::vector<int> mutation(std::vector<int>, double);
};
