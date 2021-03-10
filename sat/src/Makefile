CC=gcc
CFLAGS=-Wall -pedantic

all: sat

sat: sat_annealing.o main.o
	$(CC) -o sat.out sat_annealing.o main.o -lm

clean:
	rm *.o
