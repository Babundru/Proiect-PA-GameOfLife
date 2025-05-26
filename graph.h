#include "T2bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
	int x, y;
	int *neighbors;
	int neighbors_count;
} CELL;

typedef struct {
	int *comp_of;
	int *visited;
	int *path;
	int *best_path_nodes;
	int best_length;
	int best_start_x, best_start_y;
	CELL *nodes;
	int total_nodes;
	int component_size;
} DATA;


void make_graph(char const *matrix, int lines, int columns, FILE *output);