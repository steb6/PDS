#!/bin/bash
# prova demo con parametri fissi da 1 a 10 thread e fa la media di 10 esecuzioni ognuno

VERBOSE=1
N_NODES=10
POP_SIZE=5000
MAX_NW=10
TRIES=3
ITERATIONS=50

# ATTENZIONE rimuove risultati precedenti
rm results/*

# compila
if [[ $VERBOSE == 1 ]]
    then echo "Compiling..."
fi

g++ -Wall -g City.cpp main.cpp utilities.cpp Population.cpp GA.cpp -o demo -pthread -O3
# -fopt-info-vec
# UI
#g++ -Wall -g *.cpp -o demo -pthread -lgraph -DGRAPH -O3

SEQ_TIME=0

# per ogni mode
for((m=0; m<=2; m++))
do
    if [[ $VERBOSE == 1 ]]
        then echo "Work $m"
    fi
    FILENAME=$m'_'$N_NODES'_'$POP_SIZE'_'$ITERATIONS.dat
    >results/$FILENAME
    if [[ $m > 0 ]]
	then >results/speedup_$FILENAME
    fi
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
            TRY=$(./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS | grep loops | awk -F ' ' '{print $5}')
	    echo "$TRY"  >> aux.txt
        done
        # fai la media dei numeri in aux.txt e mettili nel results
	printf "$nw " >> results/$FILENAME
        TN=$(cat aux.txt | awk '{sum += $1} END { printf "%2.f", sum/NR }')
	echo "$TN" >> results/$FILENAME
	echo "T(N) here is $TN"

	# save speedup
	if [[ $m > 0 ]]
	    then
		printf "$nw " >> results/speedup_$FILENAME
		SPEEDUP=$(echo "scale=2; $SEQ_TIME/$TN" | bc)
		echo "SPEEDUP $SPEEDUP"
		echo "$SPEEDUP" >> results/speedup_$FILENAME
	fi
	if [[ $m == 0 ]] # To avoid to do nw times the sequential work
	then
	    break
	fi
    done
    # get sequential time
    if [[ $m == 0 ]]
	then SEQ_TIME=$(cat results/$FILENAME | awk -F ' ' '{sum += $2} END { printf "%.2f", sum/NR }')
	echo "Sequential time is $SEQ_TIME"
    fi
done

# create ideal speedup (just for graphics uses
>results/speedup_ideal.dat
for((nw=1; nw<=MAX_NW; nw++))
do
    printf "$nw " >> results/speedup_ideal.dat
    echo "$nw" >> results/speedup_ideal.dat
done


#rimuovi files ausiliari
rm aux.txt
rm demo

# plotta e apri le immagini
./graph.sh
feh results/results*.png
