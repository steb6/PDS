#!/bin/bash
# average of each execution on TRIES run

#best with 20 1000 1000
N_NODES=20
POP_SIZE=1000
MAX_NW=15
TRIES=3
ITERATIONS=1000

# remove old results
rm results/*

# compile
echo "Compiling..."
make compile

SEQ_TIME=0

# for each mode (seq, thread, ff)
for((m=0; m<=2; m++))
do
    echo "Work $m"

    FILENAME=$m'_'$N_NODES'_'$POP_SIZE'_'$ITERATIONS.dat
    >results/$FILENAME

    # if it is not sequential, create speedup file result
    if [[ $m > 0 ]]
	then >results/speedup_$FILENAME
    fi
    # do it nw times
    for((nw=1; nw<=MAX_NW; nw++))
    do
	echo "./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS"

        >aux.txt
        # execute program TRIES times and put results in aux.txt
        for((i=0; i<TRIES; i++))
        do
            TRY=$(./demo $m $N_NODES $POP_SIZE $nw $ITERATIONS | grep loops | awk -F ' ' '{print $5}')
	    echo "$TRY"  >> aux.txt
        done

        # make average of results in aux.txt and save it in T(N)
	printf "$nw " >> results/$FILENAME
        TN=$(cat aux.txt | awk '{sum += $1} END { printf "%2.f", sum/NR }')
	echo "$TN" >> results/$FILENAME
	echo "T(N) here is $TN"

	# if it is not sequential, calculate speedup
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

# create ideal speedup (just for graphical purpose)
>results/speedup_ideal.dat
for((nw=1; nw<=MAX_NW; nw++))
do
    printf "$nw " >> results/speedup_ideal.dat
    echo "$nw" >> results/speedup_ideal.dat
done


# remove useless files
rm aux.txt
rm demo

# plot results and open plots (works only in local)
make plot
feh results/results*.png
