#include "newrules.h"

int check_if_will_be_alive_new_rules(char const *matrix, int i, int j, int lines, int columns){

	const int MAX_NEIGHBORS = 8;

	int alive_cells = 0;
	int is_alive;

	int coords[2];

	const int x_positions[8] = {0, 0, 1, -1, -1, 1, -1, 1};
	const int y_positions[8] = {1, -1, 0, 0, -1, 1, 1, -1};

	if(matrix[i * columns + j]=='X'){
		is_alive = 1;
	} else {
		is_alive = 0;
	}

	// Checks the neighbors
	for(int k = 0; k < MAX_NEIGHBORS; k++){
		coords[0] = i+x_positions[k];
		coords[1] = j+y_positions[k];

		// Checks if the coordinates are within boundries
		if(coords[0]>=0 && coords[0]<lines && coords[1]>=0 && coords[1]<columns){

			// Checks if the neighbors are alive
			if(matrix[coords[0] * columns + coords[1]] == 'X'){
				alive_cells++;
			}
		}
	}

	// Returns the state of the cell
	if(is_alive == 1 || alive_cells == 2){
		return 1;
	}
	return 0;

}

TREE* set_root_coords(char const *matrix, int lines, int columns, int generations, TREE *root, FILE *output){
	
	for (int i = 0; i < lines; i++) {
		for (int j=0; j < columns; j++){

			if(matrix[i*columns+j] == 'X'){
				addCoordsAtEnd(&(root->positions), i, j);
			}

		}
	}

	//create_initial_matrix(output, lines, columns, root->positions);
	return root;
}


char* create_initial_matrix(FILE *output, int lines, int columns, COORDS *pos){

	char *matrix = malloc(sizeof(char)*lines*columns);

	for(int i=0; i<lines; i++){
		for(int j=0; j<columns; j++){
			if(i == pos->x && j == pos->y){
				matrix[i*columns + j] = 'X';
				if(pos->next != NULL){
					pos = pos->next;
				}
			} else {
				matrix[i*columns +j] = '+';
			}
		}
	}

	output_matrix(matrix, lines, columns, output);

	return matrix;
}

char* create_and_print_matrix(FILE *output, int lines, int columns, COORDS *pos, char const *prev_matrix) {

	char *matrix = malloc(sizeof(char) * lines * columns);
	memcpy(matrix, prev_matrix, lines * columns * sizeof(char));
	COORDS *current = pos;

	// Change the cells based on the coordinates in the tree;
	while (current != NULL) {
		int i = current->x;
		int j = current->y;
		if (i >= 0 && i < lines && j >= 0 && j < columns) {
			if (matrix[i * columns + j] == 'X') {
				matrix[i * columns + j] = '+';
			} else {
				matrix[i * columns + j] = 'X';
			}
		}
		current = current->next;
	}

	// Output the matrix
	output_matrix(matrix, lines, columns, output);

	return matrix;
}


void preorder(TREE* root, FILE *output, char const *prev_matrix, int lines, int columns, int first_matrix) {
	if(root){
		char *current_matrix = NULL;
		
		if(first_matrix == 1) {
			//Creates and outputs the initial matrix, for the first call of the function
			current_matrix = create_initial_matrix(output, lines, columns, root->positions);
		} else {
			current_matrix = create_and_print_matrix(output, lines, columns, root->positions, prev_matrix);
		}

		preorder(root->left, output, current_matrix, lines, columns, 0);
		preorder(root->right, output, current_matrix, lines, columns, 0);

		free(current_matrix);
	}
}

TREE* create_tree(TREE *root, int lines, int columns, int generations, char *prev_matrix, int rules, FILE *output){	//rules: 1 = new rules, 0 = old rules

	if(generations >= 0){
		char *new_matrix = malloc(sizeof(char)*lines*columns);
		//printf("Generation %d\n", generations);

		if(rules == 1){

			//printf("Enters new rules\n");
			root = malloc(sizeof(TREE));

			root->positions = NULL;

			// Creates the new matrix
			for(int i=0; i<lines; i++){
				for(int j=0; j<columns; j++){
					if(check_if_will_be_alive_new_rules(prev_matrix, i, j, lines, columns) == 1){
						new_matrix[i*columns + j] = 'X';
					} else {
						new_matrix[i*columns + j] = '+';
					}
				}
			}
			
			// Remembers the coordinates that changed
			for(int i=0; i<lines; i++){
				for(int j=0; j<columns; j++){
					if(new_matrix[i*columns + j] != prev_matrix[i*columns + j]){
						addCoordsAtEnd(&(root->positions), i, j);
					}
				}
			}
			prev_matrix = new_matrix;

		} else if(rules == 0){

			//printf("Enters Old Rules\n");

			root = malloc(sizeof(TREE));
			root->positions = NULL;

			for(int i=0; i<lines; i++){
				for(int j=0; j<columns; j++){
					if(check_if_will_be_alive(prev_matrix, i, j, lines, columns)){
						new_matrix[i*columns + j] = 'X';
						
					} else {
						new_matrix[i*columns + j] = '+';
					}
				}
			}

			for(int i=0; i<lines; i++){
				for(int j=0; j<columns; j++){
					if(new_matrix[i*columns + j] != prev_matrix[i*columns + j]){
						addCoordsAtEnd(&(root->positions), i, j);
					}
				}
			}

			prev_matrix = new_matrix;

		}

		//output_matrix(prev_matrix, lines, columns, output);

		root->left = create_tree(root->left, lines, columns, generations-1, prev_matrix, 1, output);
		root->right = create_tree(root->right, lines, columns, generations-1, prev_matrix, 0, output);

		free(new_matrix);
		return root;

	}

	return NULL;
}

void free_tree(TREE *root) {
	if (root == NULL) {
		return;
	}

	COORDS *current_coord = root->positions;
	while (current_coord != NULL) {
		COORDS *next_coord = current_coord->next;
		free(current_coord);
		current_coord = next_coord;
	}


	free_tree(root->left);
	free_tree(root->right);

	free(root);
}