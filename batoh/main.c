#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "knapsack.h"


int main(int argc, char * argv[])
{
	FILE * input_file;
	FILE * my_result;
	char * line;
	size_t len = 0;

	if(argc > 2)
	{
		input_file = fopen(argv[1], "r");
		my_result = fopen(argv[2], "w");
	}
	else
	{
		printf("pouziti: batoh input.dat output.dat\n");
		return 1;
	}

	if(input_file == NULL)
	{
		printf("chyba pri cteni souboru");
		return 1;	
	}

	double avg_error = 0, avg_bruteforce_t = 0, avg_greedy_t = 0;
	double avg_n = 0;//prumenry pocet polozek v instanci (u nasich testovacich dat vzdycky stejnej)
	double max_error = 0;
	int instances = 0;	
	
	clock_t start_bruteforce_t, end_bruteforce_t;
	clock_t start_greedy_t, end_greedy_t;
	double total_greedy_t, total_bruteforce_t;	
	
	fprintf(my_result, "id   n  bruteforce_val greedy_val bruteforce_t greedy_t error\n");
	
	//nacitani souboru po radcich
	//kazda radka predstavuje jednu instanci
	//nacteni dat, vypocet, urceni casu a chyby je tak provedeno pro kazdou instanci zvlast
	while((getline(&line, &len, input_file)) > 0)
	{
		DATA data = load_data(line);
		int i = 0;
		int bruteforce_loop = 1;
		
		//pro male instance budeme merit prumerny cas z 10 000 behu vypoctu
		if(data.m_n < 15)
			bruteforce_loop = MEASURE_LOOP;

		//mereni exaktniho algoritmu	
		RESULT result_bruteforce;
		start_bruteforce_t = clock();
		for(i = 0; i < bruteforce_loop; i++)
		{
			result_bruteforce = knapsack_bruteforce_rec(&data);	
		}
		end_bruteforce_t = clock();
		
		//mereni greedy algoritmu
		RESULT result_greedy;
		start_greedy_t = clock();
		//vzhledem ke kratke dobe vypoctu budeme brat prumerny cas z 10 000 behu
		for(i = 0; i < MEASURE_LOOP; i++)
		{
			result_greedy = knapsack_greedy(&data);
		}
		end_greedy_t = clock();
		
		//vypocet casu obou mereni
		total_bruteforce_t = (double)(end_bruteforce_t - start_bruteforce_t) / (CLOCKS_PER_SEC);
		total_greedy_t = (double)(end_greedy_t - start_greedy_t) / (CLOCKS_PER_SEC);
		
		//zprumerovani casu z vice behu
		total_greedy_t = total_greedy_t / MEASURE_LOOP;
		total_bruteforce_t = total_bruteforce_t / bruteforce_loop; //v pripade velkych instanci ==1


		avg_greedy_t = avg_greedy_t + total_greedy_t;
		avg_bruteforce_t = avg_bruteforce_t + total_bruteforce_t;

		//vypocet chyby oproti optimalnimu reseni (vysledku exaktniho algoritmu)
		double bruteforce_val = (double) result_bruteforce.m_value;
		double greedy_val = (double) result_greedy.m_value;
		double error = (bruteforce_val - greedy_val) / bruteforce_val;
		
		if(error > max_error)
			max_error = error;

		//zapis jednotlivych vysledku do souboru
		fprintf(my_result, "%d %d %.1f %.1f %.8f %.8f %f\n", 
				data.m_id, data.m_n, 
				bruteforce_val, greedy_val, 
				total_bruteforce_t, total_greedy_t,
				error);
		
		avg_error = avg_error + error;
		avg_n = avg_n + (double) data.m_n;

		//uvolneni alokovanych poli
		free(data.m_items);
		data.m_items = NULL;
		free(result_bruteforce.m_set); free(result_greedy.m_set);
		result_bruteforce.m_set = NULL; result_greedy.m_set = NULL;
		instances++;
	}

	avg_error = avg_error / (double) instances;
	avg_n = avg_n / (double) instances;
	avg_bruteforce_t = avg_bruteforce_t / (double) instances;
	avg_greedy_t = avg_greedy_t / (double) instances;

	//vypis prumeru vysledku na stdout
	printf("%s %.2f %.8f %.8f %.8f %.8f\n", argv[1], avg_n, avg_bruteforce_t, avg_greedy_t, avg_error, max_error);	

	fclose(my_result);	
	fclose(input_file);
	return 0;
}
