#include <vector>
#include <atomic>

class Population{
    private:
        int pop_size = 0;
        int n_nodes = 0;
        std::vector<std::vector<int>> population;
        std::vector<double> affinities;


    public:
	std::atomic<double> min_length;
        std::vector<int> best_one;

	Population(int, int);

	void generate_population();

	void calculate_affinities(City);
	void calculate_affinities_thread(City, int);
	void calculate_affinities_ff(City, int);

	void reproduce_all(double);
	void reproduce_all_ff(double, int);
	void reproduce_all_thread(double, int);

	std::vector<int> crossover(int, int, double);
	std::vector<int> mutation(std::vector<int>, double);

};
