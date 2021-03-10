#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "knapsack.h"

//procedura pro mereni casu algoritmu a chyby heuristiky
//vysledky jsou zapsany na stdout
void get_exact_times(FILE * input)
{
	clock_t start_bb_t, end_bb_t;
	clock_t start_dynamic_t, end_dynamic_t;
	clock_t start_greedy_t, end_greedy_t;

	double total_bb_t = 0, total_dynamic_t = 0, total_greedy_t = 0;
	double avg_bb_t = 0, avg_dynamic_t = 0, avg_greedy_t = 0;
	double max_bb_t = 0, max_dynamic_t = 0, max_greedy_t = 0;
	
	double avg_error = 0;
	int optimum = 0;
	int greedy_apx = 0;
	double avg_n = 0;

	char * line;
	size_t len = 0;
	int instances = 0;
	
	while((getline(&line, &len, input)) > 0)
	{
		DATA data = load_data(line);
		int i = 0;
		
		int measure_loop = 1;
		//opakovat mereni v cyklu pro male instance
		if(data.m_n < 15)
			measure_loop = MEASURE_LOOP;

		RESULT result_bb;
		start_bb_t = clock();
		for(i = 0; i < measure_loop; i++)
		{
			result_bb = knapsack_bb(&data);
			optimum = result_bb.m_value;
			free(result_bb.m_set);
			result_bb.m_set = NULL;
		}
		end_bb_t = clock();

		RESULT result_dynamic;
		start_dynamic_t = clock();
		for(i = 0; i < measure_loop; i++)
		{
			result_dynamic = knapsack_dynamic_value(&data);
			free(result_dynamic.m_set);
			result_dynamic.m_set = NULL;
		}
		end_dynamic_t = clock();

		RESULT result_greedy;
		start_greedy_t = clock();
		//greedy dycky pres tu merici smycku
		for(i = 0; i < MEASURE_LOOP; i++)
		{
			result_greedy = knapsack_greedy(&data);
			greedy_apx = result_greedy.m_value;
			free(result_greedy.m_set);
			result_greedy.m_set = NULL;
		}
		end_greedy_t = clock();

		total_bb_t = (double) (end_bb_t - start_bb_t) / (CLOCKS_PER_SEC);
		total_dynamic_t = (double) (end_dynamic_t - start_dynamic_t) / (CLOCKS_PER_SEC);
		total_greedy_t = (double) (end_greedy_t - start_greedy_t) / (CLOCKS_PER_SEC);

		total_bb_t = total_bb_t / (double) measure_loop;
		total_dynamic_t = total_dynamic_t / (double) measure_loop;
		total_greedy_t = total_greedy_t / (double) MEASURE_LOOP;	

		if(total_bb_t > max_bb_t)
			max_bb_t = total_bb_t;
		if(total_dynamic_t > max_dynamic_t)
			max_dynamic_t = total_dynamic_t;
		if(total_greedy_t > max_greedy_t)
			max_greedy_t = total_greedy_t;

		avg_bb_t += total_bb_t;
		avg_dynamic_t += total_dynamic_t;
		avg_greedy_t += total_greedy_t;

		double error = ((double) optimum - (double) greedy_apx) / ((double) optimum);
		avg_error += error;

		instances++;
		avg_n += (double) data.m_n;
		free(data.m_items);
		data.m_items = NULL;
	}
	
	avg_n = avg_n / (double) instances;
	avg_bb_t = avg_bb_t / (double) instances;
	avg_dynamic_t = avg_dynamic_t / (double) instances;
	avg_greedy_t = avg_greedy_t / (double) instances;

	avg_error = avg_error / (double) instances;

	printf("%.8f %.8f %.8f %.8f", avg_bb_t, avg_dynamic_t, avg_greedy_t, avg_error);
}

int main(int argc, char * argv[])
{
	FILE * input_file;

	if(argc < 2)
	{
		printf("pouziti: ./knap input_data.dat\n");
		return 1;
	}

	input_file = fopen(argv[1], "r");
	get_exact_times(input_file);
	fclose(input_file);

	return 0;
}
