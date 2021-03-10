#!/bin/bash

rm ../grafy/temp_init2.dat
#mereni zavislosti na pocatecni teplote
for i in {10..200..10}
do
	./batoh ../data/knap_40.inst.dat $i 0.999 0.001 0 >> ../grafy/temp_init2.dat
done

rm ../grafy/cool.dat
#mereni zavislosti na rychlosti ochlazovani
for i in $(seq 0.8 0.003 0.999)
do
	./batoh ../data/knap_40.inst.dat 500 $i 0.001 0 >> ../grafy/cool.dat
done

rm ../grafy/frozen.dat
#mereni zavislosti na bodu mrznuti
for i in $(seq 0.001 0.01 1)
do
	./batoh ../data/knap_40.inst.dat 500 0.999 $i 0 >> ../grafy/frozen.dat
done
