#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


struct coordinates{
	int x;
	int y;
	struct coordinates *next;
};

typedef struct coordinates COORDS;


void addCoordsAtEnd(COORDS** head, int x, int y);
void print_coords(COORDS *pos, FILE *output);