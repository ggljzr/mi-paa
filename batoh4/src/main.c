#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "knapsack.h"

//procedura pro mereni casu algoritmu a chyby heuristiky
//vysledky jsou zapsany na stdout

void get_annealing_times(FILE * input, double init_temp, double cooling, double frozen)
{
	char * line;
	size_t len = 0;
	
	clock_t start_t, end_t;
	double total_t = 0, avg_t = 0;

	double avg_error = 0;
	int instances = 0;
	
	while((getline(&line, &len, input)) > 0)
	{
		DATA data = load_data(line);
		
		start_t = clock();
		RESULT result = knapsack_annealing(&data, init_temp, cooling, frozen, 0);
		end_t = clock();

		RESULT result_bb = knapsack_bb(&data);

		total_t = (double)(end_t - start_t) / (CLOCKS_PER_SEC);
		avg_t += total_t;

		double error = ((double) (result_bb.m_value - result.m_value)) / ((double) result_bb.m_value);
		avg_error += error;

		fprintf(stderr, "%d %d %d %.8f\n", data.m_id, result.m_value, result_bb.m_value, error);
		
		free(result.m_set);free(result_bb.m_set);
		result.m_set = NULL;result_bb.m_set = NULL;
		free(data.m_items);
		data.m_items = NULL;
		instances++;
	}
	
	avg_t = avg_t / (double) instances;
	avg_error = avg_error / (double) instances;

	printf("%.8f %.8f %.8f %.8f %.8f\n",init_temp, cooling, frozen,  avg_t, avg_error);

}

//pouze na jedne instanci
void get_value_progress(FILE * input, double init_temp, double cooling, double frozen)
{
	char * line;
	size_t len = 0;

	getline(&line, &len, input);
	DATA data = load_data(line);

	RESULT result = knapsack_annealing(&data, init_temp, cooling, frozen, 1);
	
	free(result.m_set);
	result.m_set = NULL;
	free(data.m_items);
	data.m_items = NULL;
}

int main(int argc, char * argv[])
{
	FILE * input_file;

	double init_temp = 1;
	double cooling = 0.9;
	double frozen = 0.0000001;
	
	char progress = 0;
	
	if(argc < 2)
	{
		printf("pouziti: ./knap input_data.dat\n");
		return 1;
	}
	if(argc > 4)
	{
		init_temp = atof(argv[2]);
		cooling = atof(argv[3]);
		frozen = atof(argv[4]);
	}

	if(argc > 5)
	{
		progress = atoi(argv[5]);	
	}

	input_file = fopen(argv[1], "r");	
	if(progress == 0)
		get_annealing_times(input_file, init_temp, cooling, frozen);
	else
		get_value_progress(input_file, init_temp, cooling, frozen);
	fclose(input_file);

	return 0;
}
