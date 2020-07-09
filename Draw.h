#include "dependencies.h"

class Draw{
    private:
        int radius = 0;
        int n_nodes = 0;
	int top_bar = 0;
        int gd;
        int gm;

    public:
        Draw(int, int, int);
        void print_city(std::vector<int>, std::vector<int>);
        void print_best_one(std::vector<int>, std::vector<int>, std::vector<int>);
	void clear();
	void print_info(int, int, int, int, int);
	void close();
};
