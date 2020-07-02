#!/bin/bash
# prova demo con parametri fissi da 1 a 10 thread e fa la media di 10 esecuzioni ognuno

VERBOSE=1
N_NODES=10
POP_SIZE=5000
MAX_NW=10
TRIES=5
ITERATIONS=10

# ATTENZIONE rimuove risultati precedenti
rm results/*

# compila
if [[ $VERBOSE == 1 ]]
    then echo "Compiling..."
fi
g++ -Wall -g City.cpp main.cpp utilities.cpp Population.cpp -o demo -pthread

# per ogni mode
for((m=0; m<=2; m++))
do
    if [[ $VERBOSE == 1 ]]
        then echo "Work $m"
    fi
    FILENAME=$m'_'$N_NODES'_'$POP_SIZE'_'$ITERATIONS.dat
    >results/$FILENAME
    # nel range nw [1, MAX_NW]
    for((nw=1; nw<=MAX_NW; nw++))
    do
	if [[ $VERBOSE == 1 ]]
	    then echo "./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS"
	fi
        >aux.txt
        # esegui TRIES volte il programma e metti i tempi in aux.txt
        for((i=0; i<TRIES; i++))
        do
            ./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS | grep loops | awk -F ' ' '{print $5}'  >> aux.txt
        done
        # fai la media dei numeri in aux.txt e mettili nel results
	printf "$nw " >> results/$FILENAME
        cat aux.txt | awk '{sum += $1} END { print sum/NR }' >> results/$FILENAME
    done
done
#rimuovi file ausiliario
rm aux.txt
rm demo
