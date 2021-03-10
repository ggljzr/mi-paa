#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "knapsack.h"

int cost_funct(int * state, DATA * data)
{
	int i = 0;
	int sum_c = 0;
	int sum_w = 0;
	for(i = 0; i < data->m_n; i++)
	{
		sum_c += state[i] * data->m_items[i].m_value;	
		sum_w += state[i] * data->m_items[i].m_weight;
	}

	if(sum_w > data->m_max_weight)
	{
		sum_c = 0;
	}

	return sum_c;
}

int * get_rand_neighbour(int * state, int n)
{
	int item_index = rand() % n;
	
	int * new_state = (int * ) calloc(n, sizeof(int));
	int i = 0;
	for(i = 0; i < n; i++)	
	{
		new_state[i] = state[i];
	}
	new_state[item_index] = (new_state[item_index] + 1) % 2; //negace random predmetu 
	return new_state;
}

int * get_random_state(int n)
{
	int i = 0;
	int * rand_state = (int * ) calloc(n, sizeof(int));
	for(i = 0; i < n; i++)
	{
		rand_state[i] = rand() % 2;
	}
	return rand_state;
}


double acceptance(int delta, double temp)
{
	double exponent = (double) delta / temp;
	return exp(exponent);
}

RESULT knapsack_annealing(DATA * data, double init_temp, double cooling, double frozen, char value_progress)
{
	RESULT result;
	init_result(&result, data->m_n, data->m_id);

	srand(time(NULL));

	int steps = 0;

	double temp = init_temp;

	int * current = get_random_state(data->m_n);

	while(1)
	{
		temp = temp * cooling;
		if(temp < frozen)
		{
			result.m_value = cost_funct(current, data);
			break;
		}
		
		int * next = get_rand_neighbour(current, data->m_n);
		int delta = cost_funct(next, data) - cost_funct(current, data);
		
		//current = next
		if(delta > 0)
		{
			free(current);
			current = next;
			next = NULL;
		}
		else
		{
			double rnd = ((double) rand() / (RAND_MAX));
			double a = acceptance(delta, temp);
			//current = next
			if(a > rnd)
			{
				free(current);
				current = next;
				next = NULL;
			}
			//current = current;
			else
			{
				free(next);
				next = NULL;
			}	
		}
		
		//vypis prubehu ceny	
		if(value_progress)
		{
			steps++;
			printf("%d %d\n", steps, cost_funct(current, data));
		}
	}

	free(current);

	return result;
}

