CC=gcc
CFLAGS=-lm -Wall -pedantic

all: batoh

batoh: knapsack.o knapsack_2.o knapsack_3.o main.o
	$(CC) -o batoh knapsack.o knapsack_2.o knapsack_3.o main.o -lm

debug: knapsack.o knapsack_2.o knapsack_3.o main.o
	$(CC) -o batoh_dbg knapsack.o knapsack_2.o knapsack_3.o main.o -g -lm

clean:
	rm *.o
