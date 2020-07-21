compile:
	g++ -Wall main.cpp Population.cpp GA.cpp City.cpp utilities.cpp -o demo -pthread -lgraph -std=c++17 -O3

compile-graph:
	g++ -Wall *.cpp -o demo -g -pthread -lgraph -std=c++17 -DVERBOSE -DGRAPH -O3

transfer:
	rsync -azur . s.berti9@phi19:

get:
	rsync -azur s.berti9@phi19:results /home/stefano/Desktop/PDS

test:
	./test.sh

connect:
	ssh s.berti9@phi19
