#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "knapsack.h"

DATA load_data(char* line)
{
    char* token;
    int i = 0;
    int weight = 0;
    int data_i = 0;
    DATA data;
    while ((token = strsep(&line, " ")) != NULL) {
        if (i == 0) {
            data.m_id = atoi(token);
            i++;
            continue;
        }

        if (i == 1) {
            data.m_n = atoi(token);
            i++;
            //data.m_x_value = (int*)malloc(sizeof(int) * (data.m_n));
            //data.m_x_weight = (int*)malloc(sizeof(int) * (data.m_n));
            data.m_items = (ITEM*)calloc(data.m_n, sizeof(ITEM));
            continue;
        }

        if (i == 2) {
            data.m_max_weight = atoi(token);
            i++;
            continue;
        }

        if (i % 2 == 0) {
            //data.m_x_value[value_i] = atoi(token);
            int value = atoi(token);
            double ratio = (double)value / (double)weight;
            data.m_items[data_i].m_item_id = data_i;
            data.m_items[data_i].m_value = value;
            data.m_items[data_i].m_weight = weight;
            data.m_items[data_i].m_ratio = ratio;

            data_i++;
        }
        else if (i % 2 == 1) {
            //data.m_x_weight[weight_i] = atoi(token);
            weight = atoi(token);
        }
        i++;
    }
    return data;
}

void init_result(RESULT* result, int n, int id)
{
    result->m_set = (int*)calloc(n, sizeof(int));
    result->m_value = 0;
    result->m_weight = 0;
    result->m_id = id;
    result->m_n = n;
}

//tisk vysledku, pokud file==NULL na stdout, jinak do souboru
void print_result(RESULT* result, FILE* file)
{
    int i = 0;
    if (file == NULL) {
        printf("id: %d n: %d, value: %d weight: %d ", result->m_id, result->m_n,
            result->m_value, result->m_weight);
        for (i = 0; i < result->m_n; i++) {
            printf(" %d", result->m_set[i]);
        }
        printf("\n");
        return;
    }

    fprintf(file, "%d %d %d ", result->m_id, result->m_n, result->m_value);
    for (i = 0; i < result->m_n; i++) {
        fprintf(file, " %d", result->m_set[i]);
    }
    fprintf(file, "\n");
}

RESULT knapsack_bruteforce(DATA* data)
{

    int j = 0;
    long i = 0;
    long pow_n = pow(2, data->m_n);

    RESULT result;
    init_result(&result, data->m_n, data->m_id);

    for (i = 0; i < pow_n; i++) {
        int weight_sum = 0;
        int value_sum = 0;
        for (j = 0; j < data->m_n; j++) {
            if ((i >> j) & 1) //j. bit z i
            {
                weight_sum = weight_sum + data->m_items[j].m_weight;
                value_sum = value_sum + data->m_items[j].m_value;
            }
        }

        if (weight_sum <= data->m_max_weight) {
            if ((value_sum > result.m_value) || (value_sum == result.m_value && weight_sum < result.m_weight)) {
                result.m_value = value_sum;
                result.m_weight = weight_sum;
                for (j = 0; j < data->m_n; j++) {
                    result.m_set[j] = (i >> j) & 1;
                }
            }
        }
    }

    return result;
}

void knapsack_do_rec(DATA* data, RESULT* result, int current_weight, int current_value, int n,
    int* temp_result, int get)
{
    //posunuti zapisu vysledku
    //asi protoze n de do -1
    if (n < data->m_n - 1)
        temp_result[n + 1] = get;

    if (n < 0) {
        if (current_value > result->m_value && current_weight <= data->m_max_weight) {
            result->m_value = current_value;
            result->m_weight = current_weight;
            int i = 0;
            for (i = 0; i < data->m_n; i++) {
                result->m_set[i] = temp_result[i];
            }
        }
        return;
    }

    knapsack_do_rec(data, result, current_weight, current_value, n - 1, temp_result, 0);

    knapsack_do_rec(data, result,
        current_weight + data->m_items[n].m_weight,
        current_value + data->m_items[n].m_value, n - 1, temp_result, 1);
}

//wraper pro volani rekurzivni funkce knapsack_do_rec
RESULT knapsack_bruteforce_rec(DATA* data)
{
    int n = data->m_n;
    RESULT result;
    init_result(&result, data->m_n, data->m_id);

    int* temp_result = (int*)calloc(n, sizeof(int));

    knapsack_do_rec(data, &result, 0, 0, n, temp_result, 0);

    free(temp_result);

    return result;
}

int compare_items(const void* item_a, const void* item_b)
{

    double ratio_a = (*(ITEM**)item_a)->m_ratio;
    double ratio_b = (*(ITEM**)item_b)->m_ratio;

    //radim vod nejvetsiho po nejmensi
    if (ratio_a < ratio_b)
        return 1;
    if (ratio_a > ratio_b)
        return -1;
    return 0;
}

RESULT knapsack_greedy(DATA* data)
{
    //pole ukazatelu na struktury ulozene v puvodnim poli data.m_item
    ITEM** sort_array = (ITEM**)calloc(data->m_n, sizeof(ITEM*));

    RESULT result;
    init_result(&result, data->m_n, data->m_id);

    int i = 0;
    //inicializace pole
    for (i = 0; i < data->m_n; i++) {
        sort_array[i] = &data->m_items[i];
        //printf("value: %d weight: %d ratio: %f\n", sort_array[i]->m_value,
        //	       	sort_array[i]->m_weight, sort_array[i]->m_ratio);
    }

    qsort(sort_array, data->m_n, sizeof(ITEM*), compare_items);

    for (i = 0; i < data->m_n; i++) {
        //printf("w: %d, c: %d r: %f\n", sort_array[i]->m_weight,
        //		sort_array[i]->m_value, sort_array[i]->m_ratio);
        if (result.m_weight + sort_array[i]->m_weight <= data->m_max_weight) {
            result.m_weight = result.m_weight + sort_array[i]->m_weight;
            result.m_value = result.m_value + sort_array[i]->m_value;
            result.m_set[sort_array[i]->m_item_id] = 1;
        }
    }

    free(sort_array);
    return result;
}
