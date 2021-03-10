#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "knapsack.h"

//procedura pro mereni casu exaktnich algoritmu
//vysledky jsou zapsany na stdout
void get_exact_times(FILE * input)
{
	clock_t start_bruteforce_t, end_bruteforce_t;
	clock_t start_bb_t, end_bb_t;
	clock_t start_dynamic_t, end_dynamic_t;

	double total_bruteforce_t = 0, total_bb_t = 0, total_dynamic_t = 0;
	double avg_bruteforce_t = 0, avg_bb_t = 0, avg_dynamic_t = 0;
	double max_bruteforce_t = 0, max_bb_t = 0, max_dynamic_t = 0;
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

		RESULT result_bruteforce;
		start_bruteforce_t = clock();
		for(i = 0; i < measure_loop; i++)
		{
			result_bruteforce = knapsack_bruteforce_rec(&data);
			free(result_bruteforce.m_set);
			result_bruteforce.m_set = NULL;
		}
		end_bruteforce_t = clock();

		RESULT result_bb;
		start_bb_t = clock();
		for(i = 0; i < measure_loop; i++)
		{
			result_bb = knapsack_bb(&data);
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

		total_bruteforce_t = (double) (end_bruteforce_t - start_bruteforce_t) / (CLOCKS_PER_SEC);
		total_bb_t = (double) (end_bb_t - start_bb_t) / (CLOCKS_PER_SEC);
		total_dynamic_t = (double) (end_dynamic_t - start_dynamic_t) / (CLOCKS_PER_SEC);

		total_bruteforce_t = total_bruteforce_t / (double) measure_loop;
		total_bb_t = total_bb_t / (double) measure_loop;
		total_dynamic_t = total_dynamic_t / (double) measure_loop;

		if(total_bruteforce_t > max_bruteforce_t)
			max_bruteforce_t = total_bruteforce_t;
		if(total_bb_t > max_bb_t)
			max_bb_t = total_bb_t;
		if(total_dynamic_t > max_dynamic_t)
			max_dynamic_t = total_dynamic_t;

		avg_bruteforce_t += total_bruteforce_t;
		avg_bb_t += total_bb_t;
		avg_dynamic_t += total_dynamic_t;
		
		instances++;
		avg_n += (double) data.m_n;
		free(data.m_items);
		data.m_items = NULL;
	}
	
	avg_n = avg_n / (double) instances;
	avg_bruteforce_t = avg_bruteforce_t / (double) instances;
	avg_bb_t = avg_bb_t / (double) instances;
	avg_dynamic_t = avg_dynamic_t / (double) instances;

	printf("%.1f %.8f %.8f %.8f %.8f %.8f %.8f\n", avg_n, avg_bruteforce_t, max_bruteforce_t,
							      avg_bb_t, max_bb_t,
							      avg_dynamic_t, max_dynamic_t);
}

//procedura pro mereni fptas
void get_fptas_times(FILE * input, double error)
{
	clock_t start_bruteforce_t, end_bruteforce_t;
	clock_t start_fptas_t, end_fptas_t;

	double total_bruteforce_t = 0, total_fptas_t = 0;
	double avg_bruteforce_t = 0, avg_fptas_t = 0;
	double max_bruteforce_t = 0, max_fptas_t = 0;
	double avg_n;

	double avg_error = 0, max_error = 0;

	char * line;
	size_t len = 0;
	int instances = 0;

	while((getline(&line, &len, input)) > 0)
	{
		DATA data = load_data(line);

		int i = 0;
		int measure_loop = 1;

		if(data.m_n < 15)
			measure_loop = MEASURE_LOOP;

		RESULT result_bruteforce;
		start_bruteforce_t  = clock();
		for(i = 0; i < measure_loop; i++)
		{
			//tady pak dat knapsack_bruteforce_rec()
			result_bruteforce = knapsack_bb(&data);
			free(result_bruteforce.m_set);
			result_bruteforce.m_set = NULL;
		}
		end_bruteforce_t = clock();
		//vysledek pro vypocet chyby fptas algoritmu
		result_bruteforce = knapsack_bb(&data);
		//printf("OPT: ");
		//print_result(&result_bruteforce, NULL);

		RESULT result_fptas;
		start_fptas_t = clock();
		for(i = 0; i < measure_loop; i++)
		{
			result_fptas = knapsack_fptas(&data, error);
			free(result_fptas.m_set);
			result_fptas.m_set = NULL;
		}
		end_fptas_t = clock();
		//vysledek pro vypocet chyby fptas algoritmu
		result_fptas = knapsack_fptas(&data, error);
		//printf("APX: ");
		//print_result(&result_fptas, NULL);

		total_bruteforce_t = (double) (end_bruteforce_t - start_bruteforce_t) / (CLOCKS_PER_SEC);
		total_fptas_t = (double) (end_fptas_t - start_fptas_t) / (CLOCKS_PER_SEC);

		total_bruteforce_t = total_bruteforce_t / (double) measure_loop;
		total_fptas_t = total_fptas_t / (double) measure_loop;

		if(total_bruteforce_t > max_bruteforce_t)
			max_bruteforce_t = total_bruteforce_t;
		if(total_fptas_t > max_fptas_t)
			max_fptas_t = total_fptas_t;

		avg_bruteforce_t += total_bruteforce_t;
		avg_fptas_t += total_fptas_t;

		instances++;
		avg_n += (double) data.m_n;

		double bruteforce_val = (double) result_bruteforce.m_value;
		double fptas_val = (double) result_fptas.m_value;
		double error = (bruteforce_val - fptas_val) / bruteforce_val;
		avg_error += error;

		if(error > max_error)
			max_error = error;


		free(data.m_items);
		data.m_items = NULL;
		free(result_bruteforce.m_set);free(result_fptas.m_set);
		result_bruteforce.m_set = NULL; result_fptas.m_set = NULL;
	}


	avg_n = avg_n / (double) instances;
	avg_bruteforce_t = avg_bruteforce_t / (double) instances;
	avg_fptas_t = avg_fptas_t / (double) instances;
	avg_error = avg_error / (double) instances;

	printf("%.1f %.3f %.8f %.8f %.8f %0.8f\n", avg_n, error,
						avg_bruteforce_t, 
						avg_fptas_t, 
						avg_error, max_error);

}

int main(int argc, char * argv[])
{
	FILE * input_file;

	if(argc > 1)
	{

		input_file = fopen(argv[1], "r");
		if(input_file == NULL)
		{
			printf("chyba pri cteni souboru\n");
			return 1;
		}
		
		if(argc == 2)
			get_exact_times(input_file);
		else if(argc > 2)
		{
			double error = atof(argv[2]);
			get_fptas_times(input_file, error);
		}

		fclose(input_file);
		return 0;
	}

	printf("pouziti: batoh input.dat (chyba pro fptas)\n");
	return 1;


	/*			
	char * line;
	size_t len = 0;	
	
	while((getline(&line, &len, input_file)) > 0)
	{
		DATA data = load_data(line);
		RESULT result1 = knapsack_bb(&data);
		RESULT result2 = knapsack_fptas(&data, 4);
		
		printf("BRU: ");
		print_result(&result1, NULL);
		printf("APX: ");
		print_result(&result2, NULL);

		free(data.m_items);
		data.m_items = NULL;
		free(result1.m_set);
		free(result2.m_set);
		result1.m_set = NULL;
		result2.m_set = NULL;
	}*/

	get_fptas_times(input_file, 0.5);

	fclose(input_file);
		
	return 0;
}
