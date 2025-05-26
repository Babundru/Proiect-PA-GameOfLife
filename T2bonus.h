#include "newrules.h"


void print_matrix(char const *matrix, int lines, int columns, FILE *output);
void reverse(GEN *gen, char *matrix, int generations, FILE *output, int lines, int columns);
GEN *pop(GEN** top);