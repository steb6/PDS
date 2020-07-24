compile:
	g++ -Wall main.cpp Population.cpp GA.cpp City.cpp utilities.cpp MyRandom.cpp -o demo -pthread -std=c++17 -O3

compile-debug:
	g++ -Wall main.cpp Population.cpp GA.cpp City.cpp utilities.cpp MyRandom.cpp -o demo -pthread -std=c++17 -O3 -g

compile-graph:
	g++ -Wall main.cpp Population.cpp GA.cpp City.cpp utilities.cpp MyRandom.cpp Draw.cpp -o demo -g -pthread -lgraph -std=c++17 -DVERBOSE -DGRAPH -O3

vectorized:
	g++ -Wall -g City.cpp main.cpp utilities.cpp MyRandom.cpp Population.cpp GA.cpp -o demo -pthread -O3 -fopt-info-vec

transfer:
	rsync -azur . s.berti9@phi19:

get:
	rsync -azur s.berti9@phi19:results /home/stefano/Desktop/PDS

test:
	./test.sh

connect:
	ssh s.berti9@phi19

latex:
	pdflatex report/report.tex
