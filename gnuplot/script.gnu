#@IgnoreInspection BashAddShebang

col = "Age"
col2 = "lastvoted"
stats '../gnuplot/w4activedata' using col name "Aa" nooutput
stats '../gnuplot/w4activedata' using col2 name "la" nooutput
stats '../gnuplot/w4data' using col name "A" nooutput
stats '../gnuplot/w4data' using col2 name "l" nooutput
mina = Aa_min
maxa = Aa_max
minla = la_min
maxla = la_max
set boxwidth 1
set style fill pattern 4
set tics out nomirror
set ylabel 'counts'
set multiplot layout 2,1 title 'Age (ward4)'
#set title 'Age'
#unset key
set key left
set yrange [0:*]
set xrange [mina-1:maxa+1]
plot '../gnuplot/w4data' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'all', \
     '../gnuplot/w4activedata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'active'
set xrange [minla-1:maxla+1]
#set style fill pattern 5
set xrange [mina-1:40]
#plot '../gnuplot/w4activedata' using (column(col2)):(1) smooth frequency with boxes lc 2 title col2
plot '../gnuplot/w4data' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'all', \
     '../gnuplot/w4activedata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'active'
unset multiplot
pause -1 "Hit return to continue"
