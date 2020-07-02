i=1
for FILE in results/*.dat; do
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
	command=$command\"$FILE\"" with linespoints linestyle $i title \"$title\", "
	i=$i+1
done
echo "$command"
gnuplot <<- EOF
set xlabel "# Workers"
set ylabel "Time (seconds)"
set term png
set output "results/results.png"
plot $command
EOF

