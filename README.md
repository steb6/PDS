To compile
- make compile

To execute
- sequential version: ./demo 0 #NODES #POPULATION 0 #ITERATIONS
- thread version: ./demo 1 #NODES #POPULATION 1 #ITERATIONS
- fastflow version: ./demo 2 #NODES #POPLATION 2 #ITERATIONS

To execute graphical version (NOTE: you have to install the graphics.h library)
- make compile-graph
- ./demo 0 #NODES #POPULATION 0 #ITERATIONS

To compile debug version
- make compile-debug

To compile with inforation about vectorized loops
- make vectorized

To test the application (the parameters can be modified in the bash/test.sh file)
- make test

Example of usage
- ./demo 1 20 1000 4 1000
