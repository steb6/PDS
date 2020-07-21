#include "dependencies.h"

GA::GA(City& c, double r, int w, int n, int p, int i) : city(c){
    resistence = r;
    nw = w;
    n_nodes = n;
    pop_size = p;
    iterations = i;
}

// La grandezza della popolazione rimane la stessa, ma riduco il numero di iterazioni
#ifdef GRAPH
void GA::evolution_thread(Draw draw){
#else
void GA::evolution_thread(){
#endif

    int pop_thread = pop_size/nw;

    std::vector<std::thread> threads;

    #ifdef GRAPH
    std::mutex mtx;
    #endif
    if(iterations<nw){ // if we have 3 iterations and 4 workers, we have to do at least 1 cycle
	std::cout << "ERROR: less iterations than worker, what should we do?" << std::endl;
	iterations = nw;
    }

    // define threads behaviour
    #ifdef GRAPH
    auto myJob = [this, pop_thread, &draw, &mtx](int k) {
    #else
    auto myJob = [this, pop_thread](int k) {
    #endif

	std::cout << "Hi, im thread " << k << std::endl;

	Population population(pop_thread, n_nodes);
	population.generate_population();
	population.calculate_affinities(city);

	std::vector<std::vector<int>> new_population = std::vector<std::vector<int>>(pop_thread);	
	std::vector<double> new_affinities = std::vector<double>(pop_thread);

	double best_score = DBL_MAX;
	std::vector<int> best_path;

	//variables of the cycle
	double sum=0;
	int i=0;
	double score=0;

	for(int l=0; l<iterations; l++){ // because otherwise it does iterations+nw loops
	    sum=0;

	    // create new population and calculate score, then set it as population actual attribute
	    for(i=0; i<pop_thread; i++){
	        new_population[i] = population.crossover(pick_candidate(population.affinities), pick_candidate(population.affinities), resistence);
	        score = city.path_length(new_population[i]);
	        if(score<best_score){
		    best_score = score;
		    best_path = new_population[i];
		    #ifdef GRAPH //TODO così disegna anche se il nuovo percorso non è migliore del precedente
		    mtx.lock();
	            draw.clear();
	            draw.print_city(city.x, city.y);
	            draw.print_best_one(best_path, city.x, city.y);
		    mtx.unlock();
		    #endif
	        }
	        new_affinities[i] = 1/(score+1);
	        sum += new_affinities[i];
	    }
	    // normalize
	    for(i=0; i<pop_thread; i++){
	        new_affinities[i] = new_affinities[i]/sum;
    	    }
	    // evolve
	    population.population = new_population;
	    population.affinities = new_affinities;
        }
    };

    // start threads
    for(int t=0; t<nw; t++)
        threads.push_back(std::thread(myJob, t));
    for(int t=0; t<nw; t++)
        threads[t].join();

    std::cout << "Every single of the " << nw << " threads executed " << iterations << "with a population of " << pop_thread << std::endl;
}

#ifdef GRAPH
void GA::evolution_seq(Draw draw){
#else
void GA::evolution_seq(){
#endif
    int counter=0;

    Population population(pop_size, n_nodes);
    population.generate_population();
    population.calculate_affinities(city);
	
    std::vector<std::vector<int>> new_population = std::vector<std::vector<int>>(pop_size);	
    std::vector<double> new_affinities = std::vector<double>(pop_size);

    double best_score = DBL_MAX;
    std::vector<int> best_path;

    double sum=0;
    double score=0;
    int i=0;

    while(counter < iterations){
	sum=0;
	for(i=0; i<pop_size; i++){
	    new_population[i] = population.crossover(pick_candidate(population.affinities), pick_candidate(population.affinities), resistence);
	    score = city.path_length(new_population[i]);
	    if(score<best_score){
		best_score = score;
		best_path = new_population[i];
		#ifdef GRAPH
	        draw.clear();
	        draw.print_city(city.x, city.y);
	        draw.print_best_one(best_path, city.x, city.y);
		#endif
	    }
	    new_affinities[i] = 1/(score+1);
	    sum += new_affinities[i];
	}
	for(i=0; i<pop_size; i++){
	    new_affinities[i] = new_affinities[i]/sum;
    	}
	population.population = new_population;
	population.affinities = new_affinities;
	
        counter++;
    }
}

#ifdef GRAPH
void GA::evolution_ff(Draw draw){
#else
void GA::evolution_ff(){
#endif

    struct firstStage: ff_node_t<float> {
        firstStage(const size_t length):length(length) {}
        float* svc(float *) {
            for(size_t i=0; i<length; ++i) {
                ff_send_out(new float(i));
            }
            return EOS;
        }
        const size_t length;
    };
    struct secondStage: ff_node_t<float> {
        float* svc(float * task) { 
            float &t = *task;
        
            std::cout << "secondStage" << get_my_id() << " received " << t << "\n";

            t = t*t;
            return task; 
        }
    };
    struct thirdStage: ff_node_t<float> {
        float* svc(float * task) { 
            float &t = *task;
            //std::cout<< "thirdStage received " << t << "\n";
            sum += t; 
            delete task;
            return GO_ON; 
        }
        void svc_end() { std::cout << "sum = " << sum << "\n"; }
        float sum = 0.0;
    };

    const size_t nworkers = 5;
    firstStage  first(1000);
    thirdStage  third;

    ff_Farm<float> farm(
                        [nworkers]() {
            std::vector<std::unique_ptr<ff_node>> W;
            for(size_t i=0;i<nworkers;++i)
                W.push_back(make_unique<secondStage>());
            return W;
        } ()
                        );
 
    ff_Pipe<> pipe(first, farm, third);

    ffTime(START_TIME);
    if (pipe.run_and_wait_end()<0) {
        error("running pipe");
    }
    ffTime(STOP_TIME);
    std::cout << "Time: " << ffTime(GET_TIME) << "\n";

}
// ********************************* FASTFLOW STUFF /**********************************/
/*
void Population::calculate_affinities_ff(City city, int nw){
    double sum=0;
    ParallelForReduce<double> pfr(nw);
    pfr.parallel_reduce(sum, 0, //reduction variable, identity-value
			0, population.size(), //first, last
			1, 0, //step, chunksize
			[&city, this](const long i, double &mysum){ //mapF
			    double score = city.path_length(population[i]);
			    if(score < min_length){ // could cause problem, understand how to do it
				min_length = score;
				best_one = population[i];
			    }
			    score = 1/(score+1);
			    affinities[i] = score;	    
			    mysum += score;
			},
			[](double &s, const double e){ s+= e; }, //reduceF
			nw); // number of threads, we have 5

    // normalization
    ParallelFor pf(nw);
    pf.parallel_for_idx(0, pop_size,
			1, 0, //step, chunksize
			[this, sum](const long begin, const long end, const long thid)  {
			    #pragma GCC ivdep
			    for(long i=begin; i<end; ++i){ //TODO VECTORIZED but possible aliasing
                                affinities[i] = affinities[i]/sum;
			    }
			});
}

void Population::reproduce_all_ff(City city, double resistence, int nw){

    std::vector<std::vector<int>> newborn = std::vector<std::vector<int>>(pop_size);
    std::vector<double> new_affinities = std::vector<double>(pop_size);
    double sum = 0;

    ParallelForReduce<double> pfr(nw);
    pfr.parallel_reduce(sum, 0, //reduction variable, identity-value
			0, population.size(), //first, last
			1, 0, //step, chunksize
			[this, &city, &newborn, &new_affinities, resistence](const long i, double &mysum){ //mapF
			    newborn[i] = crossover(pick_candidate(affinities), pick_candidate(affinities), resistence);
			    double score = city.path_length(population[i]);
			    if(score < min_length){ // TODO could cause problem, understand how to do it
				min_length = score;
				best_one = population[i];
			    }
			    score = 1/(score+1);
			    new_affinities[i] = score;	    
			    mysum += score;
			},
			[](double &s, const double e){ s+= e; }, //reduceF
			nw); // number of threads, we have 5

    // normalization
    ParallelFor pf(nw);
    pf.parallel_for_idx(0, pop_size,
			1, 0, //step, chunksize
			[this, sum, &new_affinities](const long begin, const long end, const long thid)  {
			    #pragma GCC ivdep
			    for(long i=begin; i<end; ++i){// TODO VECTORIZED but versioned
                                new_affinities[i] = new_affinities[i]/sum;
			    }
			});
    // evolve
    affinities = new_affinities;
    population = newborn;
}
*/
