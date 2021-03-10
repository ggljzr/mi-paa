set title "Doby výpočtu exaktních algoritmů"
set key top left box

set autoscale
set logscale y
set xtics (4,10,15,20,22,25,27,30)
set ytics auto

set ylabel "Čas (sekundy)"
set xlabel "Velikost instance (n)"

set xrange[1:32]
set grid lt 0 lw 1 lc rgb "#333333"

set terminal pngcairo font ',8' enhanced
set output 'exact_times.png'

plot "exact_times.dat" using 1:2 title "hrubá síla" with points pointtype 7,\
		     "" using 1:4 title 'branch \& bound' with points pointtype 7,\
	  	     "" using 1:6 title "dekompozice podle ceny" with points pointtype 7

set title "Doby výpočtu FPTAS algoritmu"
set output 'apx_times.png'

plot "apx_times.dat" using 1:2 title "hrubá síla" with points pointtype 7,\
		  "" using 1:3 title "eps = 0.1" with points pointtype 7,\
		  "" using 1:4 title "eps = 0.3" with points pointtype 7,\
		  "" using 1:5 title "eps = 0.6" with points pointtype 7,\
		  "" using 1:6 title "eps = 0.9" with points pointtype 7

set title "Průměrná chyba FPTAS algoritmu"
set output 'apx_error.png'
set key top right box

plot "apx_error.dat" using 1:2 title "eps = 0.1" with points pointtype 7,\
		  "" using 1:4 title "eps = 0.3" with points pointtype 7,\
		  "" using 1:6 title "eps = 0.6" with points pointtype 7,\
		  "" using 1:8 title "eps = 0.9" with points pointtype 7

set title "Maximální chyba FPTAS algoritmu"
set output 'apx_max_error.png'

plot "apx_error.dat" using 1:3 title "eps = 0.1" with points pointtype 7,\
		  "" using 1:5 title "eps = 0.3" with points pointtype 7,\
		  "" using 1:7 title "eps = 0.6" with points pointtype 7,\
		  "" using 1:9 title "eps = 0.9" with points pointtype 7


