#include "coords.h"


void addCoordsAtEnd(COORDS** head, int x, int y) {
	COORDS *newCoords = (COORDS*)malloc(sizeof(COORDS));
	newCoords->x = x; 
	newCoords->y = y;
	newCoords->next = NULL;

	if (*head == NULL) {
		*head = newCoords;
	} else {
		COORDS *aux = *head;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newCoords;
	}

}

void print_coords(COORDS *pos, FILE *output) {
	while (pos != NULL) {
		fprintf(output, " %d %d", pos->x, pos->y);
		pos = pos->next;
	}
}

