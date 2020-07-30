compile:
	g++ -Wall source/main.cpp source/Population.cpp source/GA.cpp source/City.cpp source/utilities.cpp source/MyRandom.cpp -o demo -pthread -std=c++17 -O3

compile-debug:
	g++ -Wall source/main.cpp source/Population.cpp source/GA.cpp source/City.cpp source/utilities.cpp source/MyRandom.cpp -o demo -pthread -std=c++17 -O3 -g

compile-graph:
	g++ -Wall source/main.cpp source/Population.cpp source/GA.cpp source/City.cpp source/utilities.cpp source/MyRandom.cpp source/Draw.cpp -o demo -pthread -lgraph -std=c++17 -O3 -DVERBOSE -DGRAPH

compile-vectorized:
	g++ -Wall source/main.cpp source/Population.cpp source/GA.cpp source/City.cpp source/utilities.cpp source/MyRandom.cpp -o demo -pthread -std=c++17 -O3 -fopt-info-vec

test:
	bash/test.sh

plot:
	bash/plot.sh
	feh results/results*

# commands for remote machine
connect:
	ssh s.berti9@phi19

transfer:
	rsync -azur . s.berti9@phi19:

get:
	rsync -azur s.berti9@phi19:results .
