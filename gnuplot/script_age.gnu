
col = "age"
stats '../gnuplot/votehisdata4' using col nooutput
min = STATS_min
max = STATS_max
set boxwidth 1
set style fill pattern 4
#set style fill transparent solid 0.5
set tics out nomirror
set ylabel '# of voters'
set xlabel 'age in 2017'
set title 'CD1active'
#unset key
#set key left
set yrange [0:*]
set xrange [17.5:max+1]
plot '../gnuplot/votehisdata4' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted', \
     '../gnuplot/votehisdata3' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'never voted', \
     '../gnuplot/votehisdata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted 2016, 2017 only', \
     '../gnuplot/votehisdata2' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted 2017 only'
pause -1 "Hit return to continue"
