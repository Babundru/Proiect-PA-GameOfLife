#include "matrix.h"

struct tree{
	COORDS* positions;
	struct tree *left;
	struct tree *right;
};

typedef struct tree TREE;


int check_if_will_be_alive_new_rules(char const *matrix, int i, int j, int lines, int columns);
TREE* set_root_coords(char const *matrix, int lines, int columns, int generations, TREE *root, FILE *output);
char* create_initial_matrix(FILE *output, int lines, int columns, COORDS *pos, int testnr);
TREE* create_tree(TREE *root, int lines, int columns, int generations, char *prev_matrix, int rules, FILE *output);
void preorder(TREE* root, FILE *output, char const *prev_matrix, int lines, int columns, int first_matrix, int testnr);
char* create_and_print_matrix(FILE *output, int lines, int columns, COORDS *pos, char const *prev_matrix, int testnr);
void free_tree(TREE *root);