set title "Závislost exaktních algoritmů na maximální ceně"
set key bottom left box

set autoscale
set xtics auto
set logscale y
set ytics auto

set ylabel "Čas (sekundy)"
set xlabel "Maximální cena"

set grid lt 0 lw 1 lc rgb "#333333"

set terminal pngcairo font ',8' enhanced
set output 'max_price_times.png'

plot "max_price.dat" using 1:3 title 'branch \& bound' with points pointtype 7,\
		     "" using 1:4 title 'dekompozice podle ceny' with points pointtype 7,\
	  	    # "" using 1:5 title "greedy heuristika" with points pointtype 7

set output 'max_price_times20000.png'
plot "max_price20000.dat" using 1:3 title 'branch \& bound' with points pointtype 7,\
		     "" using 1:4 title 'dekompozice podle ceny' with points pointtype 7,\

set title "Závislost exaktních algoritmů na maximální váze"

set xlabel "Maximální váha"
set output 'max_weight_times.png'

plot "max_weight.dat" using 1:3 title 'branch \& bound' with points pointtype 7,\
		     "" using 1:4 title 'dekompozice podle ceny' with points pointtype 7,\

set title 'Porovnání branch \& bound algoritmů (maximální váha)'
set output 'bb_compare_w.png'

plot "max_weight.dat" using 1:3 title 's kontrolou váhy' with points pointtype 7,\
     "max_weight_modbb.dat" using 1:3 title 'bez kontroly váhy' with points pointtype 7,



set title "Závislost heuristiky na maximální váze"

unset logscale y
set output 'greedy_weight.png'

plot "max_weight.dat" using 1:5 title 'greedy heuristika' with points pointtype 7,\



set title "Závislost heuristiky na maximální ceně"

set xlabel "Maximální cena"
set output 'greedy_price.png'

plot "max_price.dat" using 1:5 title 'greedy heuristika' with points pointtype 7,\

set logscale y

set title "Závislost exaktních algoritmů na poměru kapacity a sumární váhy"
set output 'ratio1.png'

set xlabel "Poměr sumární váha/kapacita"
set format x "%.2f"

plot "ratio1.dat" using 1:3 title 'branch \& bound' with points pointtype 7,\
		"" using 1:4 title 'dekompozice podle ceny' with points pointtype 7

set title 'Závislost implementace branch \& bound algoritmu'
set output 'ratiobb.png'

set key top right box

plot "ratio25_1.dat" using 1:3 title 's dalším prořezáváním podle kapacity' with points pointtype 7,\
     "ratio25_2.dat" using 1:3 title 'bez prořezávání podle kapacity' with points pointtype 7,\

set title "Závislost exaktních algoritmů na granularitě"

set xlabel "Exponent granularity"
set output "gran1.png"

plot "gran1.dat" using 1:3 title 'branch \& bound (více velkých věcí)' with points pointtype 7,\
              "" using 1:4 title 'dekompozice podle ceny (více velkých věcí)' with points pointtype 7,\
     "gran2.dat" using 1:3 title 'branch \& bound (více malých věcí)' with points pointtype 7,\
	      "" using 1:4 title 'dekompozice podle ceny (více malých věcí)' with points pointtype 7 

set title 'Závislost greedy heuristiky na poměru kapacita/maximální váha'
set output 'ratio_greedy.png'
set xlabel "Poměr sumární váha/kapacita"
unset logscale y

plot "ratio1.dat" using 1:5 title 'greedy heuristika' with points pointtype 7

set title 'Vliv maximální váhy na chybu heuristiky'
set output 'werror.png'

unset key
set xlabel "Maximální váha"
set ylabel "Relativní chyba"
unset format x

plot "max_weight.dat" using 1:6 with points pointtype 7

set title "Vliv maximální ceny na chybu heuristiky"
set xlabel "Maximální cena"
set output "perror.png"

plot "max_price.dat" using 1:6  with points pointtype 7

set title "Vliv poměru kapacity a sumární váhy na chybu heuristiky"

set key top right box
set xlabel "Poměr sumární váha/kapacita"
set output "merror.png"

plot "ratio1.dat" using 1:6 title "rovnoměrně" with points pointtype 7,\
     "ratio_d-1.dat" using 1:6 title "více malých věcí" with points pointtype 7,\
     "ratio_d1.dat" using 1:6 title "více velkých věcí" with points pointtype 7

set title "Vliv granularity na chybu heuristiky"
set output "gerror.png"
set xlabel "Exponent granularity"

plot "gran1.dat" using 1:6 title "více velkých věcí" with points pointtype 7,\
     "gran2.dat" using 1:6 title "více malých věcí" with points pointtype 7
