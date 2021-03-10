CC=gcc
CFLAGS=-lm -Wall -pedantic

all: batoh

batoh: knapsack.o main.o
	$(CC) -o batoh knapsack.o main.o -lm

clean:
	rm *.o
