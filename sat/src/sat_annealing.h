#ifndef SAT_H_
#define SAT_H_

//struktura reprezentujici zadani
typedef struct FORMULA
{
	int m_var_num; //pocet promennych
	int m_clause_num; //pocet klauzuli
	int ** m_matrix; //matice klauzuli a literalu (prave 3 literaly na klauzuli)
	int * m_weights; //vahy promennych
}FORMULA;

typedef struct RESULT
{
	int m_value; //hodnota reseni
	char * m_var_vals; //ohodnoceni promennych
}RESULT;


FORMULA load_instance(char * path);
void free_formula(FORMULA * formula);
void print_formula(FORMULA * formula, FILE * file);
RESULT sat_annealing(FORMULA * formula, double init_temp, double cooling, double frozen);

//funkce pro nalezeni optimalniho reseni, vhodna pouze pro maly pocet promennych
RESULT sat_bruteforce(FORMULA * formula);

#endif
