set title "Relativní chyba heuristiky"
set key bottom left box

set autoscale
set xtics auto
set ytics auto

set ylabel "Relativní chyba"
set xlabel "Koeficient ochlazování"

set grid lt 0 lw 1 lc rgb "#333333"

set terminal pngcairo font ',8' enhanced
set output '18_40_cooling_e.png'

plot "18_36_cooling_e.dat" using 1:2 title "18 proměnných, 36 klauzulí" with points pointtype 7,\
     "18_40_cooling_e.dat" using 1:2 title "18 proměnných, 40 klauzulí" with points pointtype 7,\
     "18_44_cooling_e.dat" using 1:2 title "18 proměnných, 44 klauzulí" with points pointtype 7,\
     "18_48_cooling_e.dat" using 1:2 title "18 proměnných, 48 klauzulí" with points pointtype 7,\

set title "Doba výpočtu heuristiky"

set output '18_40_cooling_t.png'
set logscale y

plot "18_36_cooling_e.dat" using 1:3 title "18 proměnných, 36 klauzulí" with points pointtype 7,\
     "18_40_cooling_e.dat" using 1:3 title "18 proměnných, 40 klauzulí" with points pointtype 7,\
     "18_44_cooling_e.dat" using 1:3 title "18 proměnných, 44 klauzulí" with points pointtype 7,\
     "18_48_cooling_e.dat" using 1:3 title "18 proměnných, 48 klauzulí" with points pointtype 7,\


set title "Vývoj kvality řešení"
set ylabel "Hodnota nalezeného řešení"

set output '50_60_cooling_v.png'
unset logscale y

set key top right box

plot "50_60_cooling_t.dat" using 1:2 title "50 proměnných, 60 klauzulí, maximum z 10" with points pointtype 7,\
     "50_60_cooling_t.dat" using 1:3 title "50 proměnných, 60 klauzulí, průměr z 10" with points pointtype 7

set output '50_60_cooling2_v.png'
set key bottom right

plot "50_60_cooling2_t.dat" using 1:2 title "50 proměnných, 60 klauzulí, maximum z 10" with points pointtype 7,\
			 "" using 1:3 title "50 proměnných, 60 klauzulí, průměr z 10" with points pointtype 7

set output '40_48_cooling2_v.png'

plot "40_48_cooling2_t.dat" using 1:2 title "40 proměnných, 48 klauzulí, maximum z 10" with points pointtype 7,\
			 "" using 1:3 title "40 proměnných, 48 klauzulí, průměr z 10" with points pointtype 7

set output '40_48_cooling_v.png'

plot "40_48_cooling_t.dat" using 1:2 title "40 proměnných, 48 klauzulí, maximum z 10" with points pointtype 7,\
			 "" using 1:3 title "40 proměnných, 48 klauzulí, průměr z 10" with points pointtype 7

set title "Úspěšnost při rostoucím poměru klauzulí a proměnných (pro 40 a 50 proměnných)"
set key top right

set output '40_success.png'

set xlabel "Poměr klauzulí a proměnných"
set ylabel "Úspěšnost (%)"

plot "40_success.dat" using 1:2 title "40 proměnných, ochlazování 0.9999" with points pointtype 7,\
     "50_success.dat" using 1:2 title "50 proměnných, ochlazování 0.9999" with points pointtype 7
