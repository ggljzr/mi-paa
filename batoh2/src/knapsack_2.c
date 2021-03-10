#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "knapsack.h"

void bb_do(DATA * data, RESULT * result, int * max_price, int current_weight, int current_value, 	    int n, int * temp_result, int get)
{
	//posunuti zapisu vysledku
	//asi protoze n de do -1
	if(n < data->m_n-1)
		temp_result[n+1] = get;

	if(n < 0)
	{
		if(current_value > result->m_value && current_weight <= data->m_max_weight)
		{
			result->m_value = current_value;
			result->m_weight = current_weight;
			int i = 0;
			for(i = 0; i < data->m_n;i++)
			{
				result->m_set[i] = temp_result[i];
			}
		}
		return;
	}

	if(current_value + max_price[n] < result->m_value)
		return;

	bb_do(data, result, max_price, current_weight, current_value, n - 1, temp_result, 0);

	
	//pokud dalsi vec nepretizi batoh
	if(current_weight + data->m_items[n].m_weight <= data->m_max_weight)
	{
		bb_do(data, result, max_price,
				current_weight + data->m_items[n].m_weight,
				current_value + data->m_items[n].m_value, n - 1, temp_result, 1);
	}
}

//wraper pro volani rekurzivni funkce knapsack_do_rec
RESULT knapsack_bb(DATA * data)
{
	int n = data->m_n;
		RESULT result;
	init_result(&result, data->m_n, data->m_id);

	int * temp_result = (int * ) calloc (n, sizeof(int));

	//predpocitane max ceny od veci i do 0
	int * max_price = (int * ) calloc(n, sizeof(int));
	
	int i = 0;
	
	max_price[0] = data->m_items[0].m_value;
	for(i = 1; i < n; i++)
	{
		max_price[i] = max_price[i - 1] + data->m_items[i].m_value;		
	}
	
	bb_do(data, &result, max_price, 0, 0, n, temp_result, 0);

	free(temp_result);
	free(max_price);

	return result;
}

//dekompozice podle vahy
//tech poli musi bejt (n+1) * (m_max_weight + 1)
RESULT knapsack_dynamic_weight(DATA * data)
{
	RESULT result;
	init_result(&result, data->m_n, data->m_id);

	int i = 0;

	//dvojrozmerne pole 
	int ** weight_decomp;

	//vsechny mozne ceny od 0 vcetne
	weight_decomp = (int **) calloc (data->m_n + 1, sizeof(int *));
	
	//alokace tabulky
	for(i = 0; i < data->m_n + 1; i++)
	{
		int j = 0;
		weight_decomp[i] = (int *) calloc(data->m_max_weight + 1, sizeof(int));
		for(j = 0; j <= data->m_max_weight; j++)
		{
			weight_decomp[i][j] = 0;
		}
	}

	//vyplnovani tabulky
	for(i = 0; i < data->m_n; i++)
	{
		int j = 0;
		for(j = 0; j <= data->m_max_weight; j++)
		{
			int weight_next = j - data->m_items[i].m_weight;
			int val_next = data->m_items[i].m_value;
			
			if(weight_next >= 0){
				weight_decomp[i + 1][j] = MAX(weight_decomp[i][j],
			       		   val_next + weight_decomp[i][weight_next]);
			}
			else
			{
				weight_decomp[i + 1][j] = weight_decomp[i][j];
			}
		}
	}	

	//najdeme v tabulce reseni s optimalni cenou
	for(i = 1; i <= data->m_max_weight; i++)
	{
		if(weight_decomp[data->m_n][i] > result.m_value)
		{
			result.m_value = weight_decomp[data->m_n][i];
			result.m_weight = i;
		}
	}

	//hledani reseni
	int k = data->m_max_weight;
	for(i = data->m_n; i > 0; i--)
	{
		if(weight_decomp[i][k] != weight_decomp[i-1][k] && k >= 0)
		{
			result.m_set[i-1] = 1;
			k = k - data->m_items[i-1].m_weight;
		}
	}

	for(i = 0; i < data->m_n + 1; i++)
	{
		free(weight_decomp[i]);
	}
	free(weight_decomp);
	
	return result;
}

RESULT knapsack_dynamic_value(DATA * data)
{
	RESULT result;
	init_result(&result, data->m_n, data->m_id);

	int i = 0;
	int max_value = 0;
	int ** value_decomp;

	value_decomp = (int **) calloc (data->m_n + 1, sizeof(int *));

	for(i = 0; i < data->m_n; i++)
	{
		max_value += data->m_items[i].m_value;
	}

	for(i = 0; i < data->m_n + 1; i++)
	{
		value_decomp[i] = (int *) calloc (max_value + 1, sizeof(int));
		int j = 0;
		for(j = 0; j <= max_value; j++)
		{
			value_decomp[i][j] = INT_MAX;
		}
	}

	value_decomp[0][0] = 0;

	for(i = 0; i < data->m_n; i++)
	{
		int j = 0;
		for(j = 0; j <= max_value; j++)
		{
			int value_next = j - data->m_items[i].m_value;
			int weight_next = data->m_items[i].m_weight;

			if(value_next >= 0){
				//v pripade preceteni intu neinkrementuj hodnotu
				if(value_decomp[i][value_next] == INT_MAX)
					weight_next = INT_MAX;
				else
					weight_next = weight_next + value_decomp[i][value_next];

				value_decomp[i + 1][j] = MIN(value_decomp[i][j],
			       		   weight_next);
			}
			else
			{
				value_decomp[i + 1][j] = value_decomp[i][j];
			}
		}
	}

	for(i = 0; i <= max_value; i++)
	{
		if(value_decomp[data->m_n][i] <= data->m_max_weight && result.m_value < i)
		{
			result.m_value = i;
			result.m_weight = value_decomp[data->m_n][i];
		}
	}

	//hledani reseni
	int k = result.m_value;
	for(i = data->m_n; i > 0; i--)
	{
		if(value_decomp[i][k] != value_decomp[i-1][k] && k >= 0)
		{
			result.m_set[i-1] = 1;
			k = k - data->m_items[i-1].m_value;
		}
	}

	for(i = 0; i < data->m_n + 1; i++)
	{
		free(value_decomp[i]);
	}
	free(value_decomp);
	
	return result;
}

RESULT knapsack_fptas(DATA * data, double error)
{
	RESULT result;
	init_result(&result, data->m_n, data->m_id);

	//pokud je chyba 0, pouzij normalni dekompozici podle ceny
	//pro hledani optimalniho reseni
	if(error <= 0)
	{
		result = knapsack_dynamic_value(data);
		return result;
	}
	
	//kopie dat s upravenou cenou, kde budeme hledat reseni
	DATA scaled_data = copy_data(data);

	int max_value = 0;
	int i = 0;

	for(i = 0; i < data->m_n; i++)
	{
		if(data->m_items[i].m_value > max_value)
			max_value = data->m_items[i].m_value;
	}

	double K = (error * (double)max_value) / (double)(data->m_n);


	for(i = 0; i < data->m_n; i++)
	{
		double float_val = (double) data->m_items[i].m_value;
		scaled_data.m_items[i].m_value = (int) floor(float_val / K);
	}

	//vypocet nad upravenymi daty
	result = knapsack_dynamic_value(&scaled_data);
	
	//prepocitani vysledku podle puvodnich dat
	result.m_value = 0;
	result.m_weight = 0;
	for(i = 0; i < data->m_n; i++)
	{
		result.m_value += data->m_items[i].m_value * result.m_set[i];
		result.m_weight += data->m_items[i].m_weight * result.m_set[i];	
	}

	free(scaled_data.m_items);
	scaled_data.m_items = NULL;

	return result;
}

