#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sat_annealing.h"

void measure_error(FORMULA * form, FILE * log_file, double init_temp, double cooling, double frozen)
{
	clock_t start = 0;
	clock_t end = 0;
	RESULT exact = sat_bruteforce(form);

	start = clock();
	RESULT annealing = sat_annealing(form, init_temp, cooling, frozen);
	end = clock();

	double error = ((double) exact.m_value - (double) annealing.m_value) / ((double) exact.m_value);
	
	fprintf(log_file, "parametry: init_temp=%f : cooling=%f : frozen=%f\n", init_temp, cooling, frozen);
	
	fprintf(log_file,"optimum: %d :", exact.m_value);
	for(int i = 0; i < form->m_var_num;i++)
		fprintf(log_file, "%d ", exact.m_var_vals[i]);
	fprintf(log_file, "\n");

	fprintf(log_file, "heuristika: %d : chyba: %f :", annealing.m_value, error);
	for(int i = 0; i < form->m_var_num;i++)
		fprintf(log_file, "%d ", annealing.m_var_vals[i]);
	fprintf(log_file, "\n");
 
	double time = ((double) end - start) / (CLOCKS_PER_SEC);
	printf("%d %d %d %d %f %f %f %f %f\n", form->m_var_num, form->m_clause_num, 
					exact.m_value, annealing.m_value,
					init_temp, cooling, frozen, error, time);
	
	print_formula(form, log_file);
		free(exact.m_var_vals);
	free(annealing.m_var_vals);

	fprintf(log_file, "- - - - -\n");
}

void measure_time(FORMULA * form, FILE * log_file, double init_temp, double cooling, double frozen)
{
	clock_t start = 0;
	clock_t end = 0;

	start = clock();
	RESULT annealing = sat_annealing(form, init_temp, cooling, frozen);
	end = clock();

	double time = ((double) end - start) / (CLOCKS_PER_SEC);

	printf("%d %d %d %f %f %f %f\n", form->m_var_num, form->m_clause_num, 
			annealing.m_value, init_temp, cooling, frozen, time);
	
	print_formula(form, log_file);
	for(int i = 0; i < form->m_var_num;i++)
		fprintf(log_file, "%d ", annealing.m_var_vals[i]);
	fprintf(log_file, " -- %d -- heuristika\n", annealing.m_value);
	fprintf(log_file, "parametry: init_temp=%f, cooling=%f, frozen=%f\n", init_temp, cooling, frozen);
	
	free(annealing.m_var_vals);

	fprintf(log_file, "- - - - -\n");

}

int main(int argc, char * argv[])
{
	if(argc < 7)
	{
		printf("pouziti: ./sat instance.dat instance_log.dat init_temp cooling frozen (e, t)\n");		
		return 0;
	}

	double init_temp = 500;
	double cooling = 0.999;
	double frozen = 0.01;

	FILE * logfile = fopen(argv[2], "a");
	
	init_temp = atof(argv[3]);
	cooling = atof(argv[4]);
	frozen = atof(argv[5]);

	FORMULA form = load_instance(argv[1]);

	if(argv[6][0] == 'e')
		measure_error(&form, logfile, init_temp, cooling, frozen);
	else if(argv[6][0] == 't')
		measure_time(&form, logfile, init_temp, cooling, frozen);
	
	fclose(logfile);
	free_formula(&form);

	return 0;
}
