set title "Doba výpočtu heuristiky"
unset key

set autoscale
set xtics (4,10,15,20,22,25,27,30)
set ytics auto

set ylabel "Čas (milisekundy)"
set xlabel "Velikost instance (n)"

set xrange[1:32]
set grid lt 0 lw 1 lc rgb "#333333"

set terminal pngcairo font ',8' enhanced
set output 'time_greedy.png'

plot "time_error.dat" using ($2):($4*1000) with points pointtype 7

set title "Doba výpočtu exaktního algoritmu"

set ylabel "Čas (sekundy) log"
set logscale y

set output 'time_bruteforce.png'

plot "time_error.dat" using 2:3 with points pointtype 7

set title "Průměrná chyba heuristiky"

unset logscale
set ylabel "Průměrná chyba"

set output 'avg_error.png'

plot "time_error.dat" using 2:5 with points pointtype 7

set title "Maximální chyba heuristiky"
set ylabel "Maximální chyba"

set output 'max_error.png'

plot "time_error.dat" using 2:6 with points pointtype 7

