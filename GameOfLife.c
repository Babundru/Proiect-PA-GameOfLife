#include "newrules.h"


int main(int argc, const char *argv[]) {
	FILE *input, *output;
	char *matrix, *aux_matrix;
	int lines, columns, generations, testnr;
	GEN *gen = NULL;

	input = fopen(argv[1], "rt");
	output = fopen(argv[2], "wt");

	get_input(&lines, &columns, &generations, &testnr, input);

	matrix = (char*)malloc(lines * columns * sizeof(char));
	get_matrix(matrix, input, lines, columns);

	aux_matrix = (char*)malloc(lines * columns * sizeof(char));
	copy_matrix(matrix, lines, columns, aux_matrix);

	if(testnr == 1) {
		output_matrix(matrix, lines, columns, output);
	}

	output_updated_matrix(lines, columns, generations, output, aux_matrix, testnr, &gen);

	// Free generations list
	while(gen != NULL) {
		GEN *temp_gen = gen;
		gen = gen->next;

		// Free coords list
		COORDS *temp_coord = temp_gen->pos;
		while(temp_coord != NULL) {
			COORDS *next_coord = temp_coord->next;
			free(temp_coord);
			temp_coord = next_coord;
		}

		free(temp_gen);
	}


	

	if (testnr == 3 || testnr == 4){
		TREE *root = malloc(sizeof(TREE));
		root->left = NULL;
		root->right = NULL;
		root->positions = NULL;

		root = set_root_coords(matrix, lines, columns, generations, root, output);
		create_tree(root, lines, columns, generations, matrix, -1, output);

		preorder(root, output, matrix, lines, columns, 1, testnr);

		free_tree(root);
	}
	
	fclose(input);
	fclose(output);
	free(matrix);
	free(aux_matrix);
	return 0;
}