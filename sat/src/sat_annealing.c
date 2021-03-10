#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sat_annealing.h"

void print_formula(FORMULA * formula, FILE * file)
{
	fprintf(file,"promenne: %d\nklauzule: %d\n", formula->m_var_num, formula->m_clause_num);
	for(int i = 0; i < formula->m_var_num; i++)
	{
		fprintf(file, "%d ", formula->m_weights[i]);
	}

	fprintf(file,"\n");

	for(int i = 0; i < formula->m_clause_num;i++)
	{
		for(int j = 0; j < 2; j++)
			fprintf(file, "%d ", formula->m_matrix[i][j]);
		fprintf(file, "%d\n", formula->m_matrix[i][2]);
	}
}

FORMULA load_instance(char * path)
{
	char * line;
	char * token;
	size_t len = 0;
	FILE * file = fopen(path, "r");

	int clause_index = 0;

	FORMULA formula;

	while((getline(&line, &len, file)) > 0)
	{
		if(line[0] == 'c')
			continue;

		if(line[0] == 'p')
		{
			int i = 0;
			while((token = strsep(&line, " ")) != NULL)
			{
				if(i < 2)
				{	i++;
					continue;
				}

				if(i == 2)
				{
					formula.m_var_num = atoi(token);
				}

				if(i == 3)
				{
					formula.m_clause_num = atoi(token);
				}
				i++;	
			}
			continue;		
		}

		
		if(line[0] == 'w')
		{
			formula.m_matrix = (int ** )calloc(formula.m_clause_num, sizeof(int*));
		
			for(int i = 0; i < formula.m_clause_num;i++)
			{
				formula.m_matrix[i] = (int *) calloc(3, sizeof(int));
			}

			formula.m_weights = (int *)calloc(formula.m_var_num, sizeof(int));

			int i = 0;
			while((token = strsep(&line, " ")) != NULL)
			{
				if(i == 0)
				{
					i++;
					continue;
				}
				formula.m_weights[i-1] = atoi(token);
				i++;
			}
			continue;
		}

		int i = 0;
		while((token = strsep(&line, " ")) != NULL)
		{
			char value = atoi(token);
			if(value == 0)
				continue;
			formula.m_matrix[clause_index][i] = value;
			i++;

		}

		clause_index++;
			
	}
	
	fclose(file);
	return formula;	
}

void free_formula(FORMULA * formula)
{
	for(int i = 0; i < formula->m_clause_num; i++)
	{
		free(formula->m_matrix[i]);
	}
	free(formula->m_matrix);
}

int cost_funct(FORMULA * formula, char * vals)
{
	for(int i = 0; i < formula->m_clause_num; i++)
	{
		char clause_val = 0;
		
		for(int j = 0; j < 3; j++)
		{
			int var_index = abs(formula->m_matrix[i][j]) - 1;
			char var_val = (formula->m_matrix[i][j] > 0) ? vals[var_index] : !vals[var_index];
			clause_val = (clause_val || var_val);
		}

		if(clause_val == 0)
			return 0;
	}

	int solution_val = 0;
	for(int i = 0; i < formula->m_var_num; i++)
	{
		solution_val += vals[i] * formula->m_weights[i];
	}
	return solution_val;

}

char * get_rand_neighbour(char * state, int n)
{
	int item_index = rand() % n;
	
	char * new_state = (char * ) calloc(n, sizeof(char));
	int i = 0;
	for(i = 0; i < n; i++)	
	{
		new_state[i] = state[i];
	}
	new_state[item_index] = (new_state[item_index] + 1) % 2; //negace random predmetu 
	return new_state;
}

char * get_random_state(int n)
{
	int i = 0;
	char * rand_state = (char * ) calloc(n, sizeof(char));
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

RESULT sat_annealing(FORMULA * formula, double init_temp, double cooling, double frozen)
{
	srand(time(NULL));
	RESULT result;
	result.m_value = 0;
	result.m_var_vals = (char * )calloc(formula->m_var_num, sizeof(char));

	double temp = init_temp;

	//zacneme s nahodnym ohodnocenim promennych
	char * current = get_random_state(formula->m_var_num);
	
	//zacatek s 0 stavem
	//char * current = (char * ) calloc (formula->m_var_num, sizeof(char));

	while(1)
	{
		temp = temp * cooling;
		if(temp < frozen)
		{
			result.m_value = cost_funct(formula, current);
			for(int i = 0; i < formula->m_var_num; i++)
				result.m_var_vals[i] = current[i];
			break;	
		}

		char * next = get_rand_neighbour(current, formula->m_var_num);
		int delta = cost_funct(formula, next) - cost_funct(formula, current);

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
			if(a > rnd)
			{
				free(current);
				current = next;
				next = NULL;
			}
			else
			{
				free(next);
				next = NULL;
			}
		}
	}

	free(current);

	return result;
}

//vrati int jako bitove pole charu, n je delka pole
//pro generovani ohodnoceni promennych z intu
char * int_to_bitstring(int data, int n)
{
	char * bitstring = (char *) calloc(n, sizeof(char));
	
	for(int i = 0; i < n; i++)
	{
		bitstring[i] = ((data >> i) & 1);
	}
	
	return bitstring;	
}

RESULT sat_bruteforce(FORMULA * formula)
{
	long pow_n = pow(2,formula->m_var_num);

	RESULT result;
	result.m_value = 0;
	result.m_var_vals = (char *) calloc(formula->m_var_num, sizeof(char));

	for(long i = 0; i < pow_n; i++)
	{
		char * var_vals = int_to_bitstring(i, formula->m_var_num);
		int current_value = cost_funct(formula, var_vals);
		
		if(current_value > result.m_value)
		{
			result.m_value = current_value;
			for(int i = 0; i < formula->m_var_num; i++)
				result.m_var_vals[i] = var_vals[i];
		}

		free(var_vals);
	
	}

	return result;
}

