#include "dependencies.h"

class Draw{
    private:
        int radius = 0;
        int n_nodes = 0;
        int gd;
        int gm;

    public:
        Draw(int, int);
        void print_city(std::vector<int>, std::vector<int>, std::vector<std::vector<bool>>);
        void print_best_one(std::vector<int>, std::vector<int>, std::vector<int>);
	void clear();
	void close();
};
