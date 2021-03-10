set title "Vývoj ceny při simulovaném ochlazování"
set key bottom left box

set autoscale
set xtics auto
set ytics auto

set logscale x

set ylabel "Cena"
set xlabel "Iterace"

set grid lt 0 lw 1 lc rgb "#333333"

set terminal pngcairo font ',8' enhanced
set output 'progress.png'

plot "progress.dat" using 1:2 title "T_i = 100" with points pointtype 7,\
	     "progress2.dat" using 1:2 title "T_i = 1000" with points pointtype 7,\
	     "progress3.dat" using 1:2 title "T_i = 10000" with points pointtype 7

	unset logscale x

	set title "Vliv počáteční teploty na dobu běhu"
	set xlabel "Počáteční teplota"
	set ylabel "Doba běhu (sekundy)"

	set key bottom right box

	set output 'temp_init2.png'

	plot "temp_init2.dat" using 1:4 title "Ochlazovani = 0.999, zamrznuti = 0.001" with points pointtype 7

	set title "Vliv počáteční teploty na relativní chybu"
	set ylabel "Relativní chyba"

	set key top right box

	set output 'temp_init_e2.png'

	plot "temp_init2.dat" using 1:5 title "Ochlazovani = 0.999, zamrznuti = 0.001" with points pointtype 7

	set title "Vliv rychlosti ochlazování na dobu běhu"
	set output 'cool.png'
	set ylabel "Doba běhu (sekundy)"
	set xlabel "Koeficient ochlazování"
	set logscale y

	plot "cool.dat" using 2:4 title "Počáteční teplota = 500, zamrznuti = 0.001" with points pointtype 7

	unset logscale y

set title "Vliv bodu zamrznutí na dobu běhu"
set output 'frozen.png'
set xlabel "Bod zamrznutí"

plot "frozen.dat" using 3:4 title "Počáteční teplota = 500, ochlazování = 0.999" with points pointtype 7

set ylabel "Relativní chyba"

set title "Vliv bodu zamrznutí na relativní chybu"
set output 'frozen_e.png'

plot "frozen.dat" using 3:5 title "Počáteční teplota = 500, ochlazování = 0.999" with points pointtype 7

set title "Vliv rychlosti ochlazování na relativní chybu"
set xlabel "Koeficient ochlazování"
set output 'cool_e.png'

plot "cool.dat" using 2:5 title "Počáteční teplota = 500, zamrznuti = 0.001" with points pointtype 7

 
