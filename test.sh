#!/bin/bash
# prova demo con parametri fissi da 1 a 10 thread e fa la media di 10 esecuzioni ognuno

VERBOSE=1
N_NODES=10
POP_SIZE=5000
MAX_NW=10
TRIES=3
ITERATIONS=10

# ATTENZIONE rimuove risultati precedenti
rm results/*

# compila
if [[ $VERBOSE == 1 ]]
    then echo "Compiling..."
fi

g++ -Wall -g City.cpp main.cpp utilities.cpp Population.cpp -o demo -pthread -O3
# UI
#g++ -Wall -g *.cpp -o demo -pthread -lgraph -DGRAPH -O3

# save global sequential time TODO stavo salvando gli speedup, ma il bash lavora solo con interi, quindi bisogna cambiare il main TODO ho cambiato il main, ma adesso non assegna il valore alla variabile perchè non è un intero, però la stampa TODO ME LO RICORDAVO DIVERSO LO SPEEDUP PD TODO ma se i tempi sono giusti, come mai lo speedup dei thread per esempio rimane ad uno?
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
            ./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS | grep loops | awk -F ' ' '{print $5}'  >> aux.txt
        done
        # fai la media dei numeri in aux.txt e mettili nel results
	printf "$nw " >> results/$FILENAME
        cat aux.txt | awk '{sum += $1} END { print sum/NR }' >> results/$FILENAME

	# save speedup
	if [[ $m > 0 ]]
	    then
		printf "$nw " >> results/speedup_$FILENAME
		TN=$(cat aux.txt | awk '{sum += $1} END { print sum/NR }')
		SPEEDUP=$(echo "scale=2; $SEQ_TIME/$TN" | bc)
		echo "$SPEEDUP" >> results/speedup_$FILENAME
	fi

    done
    # get sequential time
    if [[ $m == 0 ]]
	then SEQ_TIME=$(cat results/$FILENAME | awk -F ' ' '{sum += $2} END { print sum/NR }')
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
