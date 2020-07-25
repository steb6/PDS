# Plot times
i=1
for FILE in results/0*.dat results/1*.dat results/2*.dat; do
	case "$FILE" in
	results/0*)
		title="Sequential"
		;;
	results/1*)
		title="Thread"
		;;
	results/2*)
		title="FastFlow"
		;;
	*)
		title="Unknown"
		;;
	esac
	command=$command\"$FILE\"" with lines linestyle $i title \"$title\", "
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
		;;
	results/speedup_1*)
		title="Thread"
		;;
	results/speedup_2*)
		title="FastFlow"
		;;
	*)
		title="Unknown"
		;;
	esac
	command=$command\"$FILE\"" with lines linestyle $i title \"$title\", "
	i=$i+1
done
echo "$command"

gnuplot <<- EOF
set xlabel "# Workers"
set ylabel "Speedup"
set term png
set output "results/results_speedup.png"
plot $command
EOF
