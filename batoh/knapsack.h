#ifndef KNAPSACK_H_
#define KNAPSACK_H_

//pocet iteraci vypoctu pro ziskani prumerneho casu behu
#define MEASURE_LOOP 10000

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
void print_result(RESULT * result, FILE * file);

//reseni hrubou silou
//iterativni funkce se sozitosti n * 2^n
RESULT knapsack_bruteforce(DATA * data); //omezena datovym typem long (64bit => max 64 polozek)
//rekurzivni funkce se slozitosti 2^n, pouzita pro konecne mereni
RESULT knapsack_bruteforce_rec(DATA * data);

//jednoducha heuristika
RESULT knapsack_greedy(DATA * data);

#endif //KNAPSACK_H_
