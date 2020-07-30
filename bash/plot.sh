# Plot times
i=1
for FILE in results/0*.dat results/1*.dat results/2*.dat; do
	case "$FILE" in
	results/0*)
		title="Sequential"
		command=$command\"$FILE\"" with lines lt -1 title \"$title\", "
		;;
	results/1*)
		title="Thread"
		command=$command\"$FILE\"" with linespoints lt -1 pi -10 pt 7 ps 0.2 title \"$title\", "
		;;
	results/2*)
		title="FastFlow"
		command=$command\"$FILE\"" with linespoints lt -1 pi -10 pt 5 title \"$title\", "
		;;
	*)
		title="Unknown"
		command=$command\"$FILE\"" with lines linestyle $i title \"$title\", "
		;;
	esac
	#command=$command\"$FILE\"" with lines linestyle $i title \"$title\", "
	i=$i+1
done
echo "$command"

gnuplot <<- EOF
set xlabel "# Workers"
set ylabel "Time (microseconds)"
set term png
set output "results/results.png"
plot $command
EOF

# Plot speedups
command=''
i=1
for FILE in results/speedup*; do
	case "$FILE" in
	results/speedup_ideal*)
		title="Ideal"
		command=$command\"$FILE\"" with lines lt -1 title \"$title\", "
		;;
	results/speedup_1*)
		title="Thread"
		command=$command\"$FILE\"" with linespoints lt -1 pi -10 pt 7 ps 0.2 title \"$title\", "
		;;
	results/speedup_2*)
		title="FastFlow"
		command=$command\"$FILE\"" with linespoints lt -1 pi -10 pt 5 title \"$title\", "
		;;
	*)
		title="Unknown"
		command=$command\"$FILE\"" with lines title \"$title\", "
		;;
	esac
	#command=$command\"$FILE\"" with lines linestyle $i title \"$title\", "
	#i=$i+1
done
echo "$command"

gnuplot <<- EOF
set xlabel "# Workers"
set ylabel "Speedup"
set term png
set output "results/results_speedup.png"
plot $command
EOF
