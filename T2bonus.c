#include "T2bonus.h"

void print_matrix(char const *matrix, int lines, int columns, FILE *output){

	for (int i = 0; i < lines; i++) {
		for (int j=0; j< columns; j++){
			printf("%c", matrix[i*columns+j]);
		}
		printf("\n");
	}
	printf("\n");
}


GEN* copy_stack(GEN *original_top) {
	GEN *temp_stack = NULL;
	GEN *copy_top = NULL;

	for (GEN *curr = original_top; curr != NULL; curr = curr->next) {

		COORDS *coords_copy = NULL, *tail = NULL;
		for (COORDS *pc = curr->pos; pc != NULL; pc = pc->next) {
			COORDS *new_coord = (COORDS *)malloc(sizeof(COORDS));
			new_coord->x = pc->x;
			new_coord->y = pc->y;
			new_coord->next = NULL;

			if (coords_copy == NULL) {
				coords_copy = tail = new_coord;
			} else {
				tail->next = new_coord;
				tail = new_coord;
			}
		}

		GEN *new_gen = (GEN *)malloc(sizeof(GEN));
		new_gen->number = curr->number;
		new_gen->pos = coords_copy;
		new_gen->next = temp_stack;
		temp_stack = new_gen;
	}


	while (temp_stack != NULL) {

		GEN *temp = temp_stack;
		temp_stack = temp_stack->next;

		temp->next = copy_top;
		copy_top = temp;
	}

	return copy_top;
}


GEN *pop(GEN** top) {
	if (!top || !*top) return NULL;

	GEN *temp = (*top);
	GEN *aux = malloc(sizeof(GEN));
	if (!aux) return NULL;
	
	aux->number = temp->number;
	
	// Copy the entire position list
	aux->pos = NULL;
	COORDS *last = NULL;
	COORDS *current_pos = temp->pos;
	while (current_pos) {
		COORDS *new_pos = malloc(sizeof(COORDS));
		if (!new_pos) break;
		new_pos->x = current_pos->x;
		new_pos->y = current_pos->y;
		new_pos->next = NULL;
		
		if (!aux->pos) {
			aux->pos = new_pos;
		} else {
			last->next = new_pos;
		}
		last = new_pos;
		current_pos = current_pos->next;
	}
	
	*top = temp->next;
	free(temp);
	return aux;
}

void reverse(GEN *gen, char *matrix, int generations, FILE *output, int lines, int columns) {

	GEN *copied_gen = copy_stack(gen);


	for (int k = generations; k >= 0; k--) {
		GEN *current = pop(&copied_gen);
		if (!current) break;
		
		COORDS *pos = current->pos;
		while (pos) {
			if (pos->x >= 0 && pos->x < lines && pos->y >= 0 && pos->y < columns) {
				int index = pos->x * columns + pos->y;

				if(matrix[index] == 'X'){
					matrix[index] = '+';
				} else {
					matrix[index] = 'X';
				}

			}
			pos = pos->next;
		}
		
		// Free the copied positions
		while (current->pos) {
			COORDS *temp = current->pos;
			current->pos = current->pos->next;
			free(temp);
		}
		free(current);
	}
	
	print_matrix(matrix, lines, columns, output);
}