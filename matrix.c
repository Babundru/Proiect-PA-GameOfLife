#include "T2bonus.h"

void copy_matrix(char const *matrix, int lines, int columns, char *pasted_matrix){
	for(int i=0; i < lines; i++){
		for(int j=0; j< columns; j++){
			pasted_matrix[i*columns+j]=matrix[i*columns+j];
		}
	}
}

void get_input(int *lines, int *columns, int *generations, int *testnr, FILE *input){

	fscanf(input, "%d", testnr);
	fscanf(input, "%d", lines);
	fscanf(input, "%d", columns);
	fscanf(input, "%d", generations);
}


void get_matrix(char *matrix, FILE *input, int lines, int columns){

	for(int i=0; i< lines; i++){
		for(int j=0; j< columns; j++){
			fscanf(input, " %c", &matrix[i * columns + j]);
		}
	}

}

int check_if_will_be_alive(char const *matrix, int i, int j, int lines, int columns){

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
				alive_cells += 1;
			}
		}
	}

	// Returns the state of the cell
	if(is_alive){
		if(alive_cells<2){
			return 0;
		}
		if(alive_cells == 2 || alive_cells == 3){
			return 1;
		}
	} else {
		if(alive_cells == 3){
			return 1;
		}
	}
	return 0;

}

void update_matrix(char *aux_matrix, int lines, int columns, char *updated_matrix, GEN *gen){

	for(int i = 0; i < lines; i++) {
		for(int j = 0; j < columns; j++) {

			int alive = check_if_will_be_alive(aux_matrix, i, j, lines, columns);

			if (alive) {
				updated_matrix[i * columns + j] = 'X';
			} else {
				updated_matrix[i * columns + j] = '+';
			}

			if(updated_matrix[i * columns + j] != aux_matrix[i * columns + j]) {
				addCoordsAtEnd(&(gen->pos), i, j);
			}

		}
}
	copy_matrix(updated_matrix, lines, columns, aux_matrix);

}

void output_matrix(char const *matrix, int lines, int columns, FILE *output){

	for (int i = 0; i < lines; i++) {
		for (int j=0; j< columns; j++){
			fprintf(output , "%c", matrix[i*columns+j]);
		}
		fprintf(output, "\n");
	}
	fprintf(output, "\n");
}

void output_updated_matrix(int lines, int columns, int generations, FILE *output, char *aux_matrix, int testnr, GEN **gen){

	char *updated_matrix = malloc(lines * columns * sizeof(char));
	*gen = NULL;

	for(int k = 0; k < generations; k++) {
		
		push(gen, k+1);

		update_matrix(aux_matrix, lines, columns, updated_matrix, *gen);		

		copy_matrix(updated_matrix, lines, columns, aux_matrix);

		if(testnr == 1) {
			output_matrix(aux_matrix, lines, columns, output);
		}

		if(testnr == 2){
			printGen(*gen, output);
		}

	}

	
	///////////////// BONUS TASK 2 //////////////////////

	// if(testnr == 2){
	// 	reverse(*gen, updated_matrix, generations, output, lines, columns);
	// }

	////////////////////////////////////////////////////

	free(updated_matrix);
}