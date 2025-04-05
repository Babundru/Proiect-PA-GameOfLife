#include "generations.h"


void copy_matrix(char const *matrix, int lines, int columns, char *pasted_matrix);
void get_input(int *lines, int *columns, int *generations, int *testnr, FILE *input);
void get_matrix(char *matrix, FILE *input, int lines, int columns);
int check_if_will_be_alive(char const *matrix, int i, int j, int lines, int columns);
void update_matrix(char *aux_matrix, int lines, int columns, char *updated_matrix, GEN *gen);
void output_matrix(char const *matrix, int lines, int columns, FILE *output);
void output_updated_matrix(int lines, int columns, int generations, FILE *output, char *aux_matrix, int testnr, GEN **gen);