#ifndef KNAPSACK_H_
#define KNAPSACK_H_

//pocet iteraci vypoctu pro ziskani prumerneho casu behu
#define MEASURE_LOOP 10000

//min/max makra
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b


typedef struct ITEM
{
	int m_item_id; //index v poli m_items, tj puvodni pozice pri nacitani ze souboru
	int m_value;
	int m_weight;
	double m_ratio;
}ITEM;

typedef struct DATA
{
	int m_id;
	int m_n;
	int m_max_weight;
	ITEM * m_items;
}DATA;

typedef struct RESULT
{
	int m_id;
	int m_max_weight;
	int m_n;
	int m_value;
	int m_weight;
	int * m_set;
}RESULT;

//podpurne funkce
DATA load_data(char * line);
DATA copy_data(DATA * data);
void init_result(RESULT * result, int n, int id);
void print_result(RESULT * result, FILE * file);

/*
 * Ukol 1 - bruteforce a jednoducha heuristika
 * soubor: knapsack.c
 * 
 */

//reseni hrubou silou
//iterativni funkce se sozitosti n * 2^n
RESULT knapsack_bruteforce(DATA * data); //omezena datovym typem long (64bit => max 64 polozek)
//rekurzivni funkce se slozitosti 2^n, pouzita pro konecne mereni
RESULT knapsack_bruteforce_rec(DATA * data);

//jednoducha heuristika
RESULT knapsack_greedy(DATA * data);

/*
 * Ukol 2 - B&B, dynamicke programovani, FPTAS
 * soubor: knapsack_2.c
 */

//branch & bound algoritmus O(2^n)
//v podstate knapsack_bruteforce_rec doplnena o preruseni rekurzivniho volani
//pokud jiz nemohu nalezt lepsi reseni nez dosud nalezene nejlepsi reseni
//ani kdybych do batohu dal vsechny zbyvajici veci bez ohledu na vahu
//navic drobnejsi prorezavani v podobe nepridavani veci, ktere jiz prekroci vahu batohu
RESULT knapsack_bb(DATA * data); 

//dynamicky algoritmus (dekompozice podle vahy) slozitost n * max_weight
RESULT knapsack_dynamic_weight(DATA * data);
//dekompozice podle ceny, slozitost n * max_value (suma vsech cen predemtu)
RESULT knapsack_dynamic_value(DATA * data);

//dekompozice podle ceny na upravenejch datech, kde sou ceny vsech 
//predmetu polynomialne vazany na n
//slozitos je zavisla na zvoleny chybe, O(n^2 * n/error)
//pro nulovou chybu najde optimum pomoci dekompozice podle ceny
RESULT knapsack_fptas(DATA * data, double error);

#endif //KNAPSACK_H_
