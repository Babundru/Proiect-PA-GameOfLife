#include "graph.h"

int compare_xy(const void *a, const void *b){
	const CELL *na = *(CELL* const*)a;
	const CELL *nb = *(CELL* const*)b;
	if (na->x != nb->x) return na->x - nb->x;
	return na->y - nb->y;
}

int compare_paths(DATA *data, int depth){
	for (int i = 0; i <= depth; i++) {
		int curr_x = data->nodes[data->path[i]].x;
		int curr_y = data->nodes[data->path[i]].y;
		int best_x = data->nodes[data->best_path_nodes[i]].x;
		int best_y = data->nodes[data->best_path_nodes[i]].y;

		if (curr_x != best_x) return curr_x - best_x;
		if (curr_y != best_y) return curr_y - best_y;
	}
	return 0;
}

void update_best_path(DATA *data, int depth){
	data->best_length = depth + 1;
	memcpy(data->best_path_nodes, data->path, data->best_length * sizeof(int));
	data->best_start_x = data->nodes[data->path[0]].x;
	data->best_start_y = data->nodes[data->path[0]].y;
}

void DFS(DATA *data, int u, int depth, int comp_id){
	data->visited[u] = 1;
	data->path[depth] = u;

	//check if current path is better
	if (depth + 1 > data->best_length || (depth + 1 == data->best_length && compare_paths(data, depth) < 0)) {
		update_best_path(data, depth);
	}

	// Early termination conditions
	if (data->best_length == data->component_size || data->component_size <= data->best_length){
		data->visited[u] = 0;
		return;
	}

	// Visit neighbors in sorted order
	for (int i = 0; i < data->nodes[u].neighbors_count; i++) {
		int v = data->nodes[u].neighbors[i];
		if (!data->visited[v] && data->comp_of[v] == comp_id) {
			DFS(data, v, depth + 1, comp_id);
			if (data->best_length == data->component_size) break;
		}
	}

	data->visited[u] = 0;
}

void BFS(DATA *data, int start, int comp_id, int *queue) {
	int qs = 0, qe = 0;
	queue[qe++] = start;
	data->comp_of[start] = comp_id;

	while (qs < qe) {
		int u = queue[qs++];
		for (int i = 0; i < data->nodes[u].neighbors_count; i++) {
			int v = data->nodes[u].neighbors[i];
			if (data->comp_of[v] < 0) {
				data->comp_of[v] = comp_id;
				queue[qe++] = v;
			}
		}
	}
}

void make_graph(const char *matrix, int lines, int columns, FILE *output) {
	int *map = malloc(lines * columns * sizeof(int));
	memset(map, -1, lines * columns * sizeof(int));

	DATA data = {0};
	data.nodes = malloc(lines * columns * sizeof(CELL));
	data.total_nodes = 0;

	// Creates the graph
	for (int i = 0; i < lines; i++){
		for (int j = 0; j < columns; j++) {
			if (matrix[i * columns + j] == 'X') {
				CELL *n = &data.nodes[data.total_nodes];
				n->x = i;
				n->y = j;
				n->neighbors_count = 0;
				n->neighbors = NULL;
				map[i * columns + j] = data.total_nodes++;
			}
		}
	}


	// Count neighbors for each node
	for (int i = 0; i < data.total_nodes; i++) {
		int neighbor_x, neighbor_y;
		int current_x = data.nodes[i].x, current_y = data.nodes[i].y;

		// x and y offsets
		for (int ofsx = -1; ofsx <= 1; ofsx++){
			for (int ofsy = -1; ofsy <= 1; ofsy++){

				if (ofsx != 0 || ofsy != 0){
					neighbor_x = current_x + ofsx;
					neighbor_y = current_y + ofsy;

					if (neighbor_x >= 0 && neighbor_x < lines && neighbor_y >= 0 && neighbor_y < columns) {
						int v = map[neighbor_x * columns + neighbor_y];

						if (v >= 0){
							data.nodes[i].neighbors_count++;
						}
					}
				}
			}
		}

		data.nodes[i].neighbors = malloc(data.nodes[i].neighbors_count * sizeof(int));
		data.nodes[i].neighbors_count = 0;
	}

	// Fill neighbors for each node
	for (int i = 0; i < data.total_nodes; i++) {
		int neighbor_x, neighbor_y;
		int current_x = data.nodes[i].x, current_y = data.nodes[i].y;

		for (int ofsx = -1; ofsx <= 1; ofsx++){
			for (int ofsy = -1; ofsy <= 1; ofsy++){

				if (ofsx | ofsy){
					neighbor_x = current_x + ofsx;
					neighbor_y = current_y + ofsy;

					if (neighbor_x >= 0 && neighbor_x < lines && neighbor_y >= 0 && neighbor_y < columns) {
						int v = map[neighbor_x * columns + neighbor_y];

						if (v >= 0){
							data.nodes[i].neighbors[data.nodes[i].neighbors_count++] = v;
						}
					}
				}
			}
		}
	}
	//this ^ couldve been done in only one loop but it is simpler this way
	free(map);

	// no need for the map anymore since we have the adjacency list

	data.comp_of = malloc(data.total_nodes * sizeof(int));
	memset(data.comp_of, -1, data.total_nodes * sizeof(int));
	int *queue = malloc(data.total_nodes * sizeof(int));
	int comp_cnt = 0;

	for(int u = 0; u < data.total_nodes; u++) {
		if(data.comp_of[u] < 0) {
			BFS(&data, u, comp_cnt, queue);
			comp_cnt++;
		}
	}
	free(queue);


	/////////////////////////////////////////////////////////////////
	/////////////////////// PREP FOR DFS ////////////////////////////
	/////////////////////////////////////////////////////////////////


	data.visited = calloc(data.total_nodes, sizeof(int));
	data.path = malloc(data.total_nodes * sizeof(int));
	data.best_path_nodes = malloc(data.total_nodes * sizeof(int));
	data.best_length = -1;
	data.best_start_x = INT_MAX;
	data.best_start_y = INT_MAX;

	int final_best_size = -1;

	int best_start[2] = {INT_MAX, INT_MAX};
	int *global_best_path = NULL;

	for (int c = 0; c < comp_cnt; c++) {
		CELL **cells = malloc(data.total_nodes * sizeof(CELL *));
		int cells_count = 0;
		for (int u = 0; u < data.total_nodes; u++) if (data.comp_of[u] == c) cells[cells_count++] = &data.nodes[u];
		data.component_size = cells_count;

		if (cells_count == 0) {
			free(cells);
			continue;
		}

		//sort for the output
		qsort(cells, cells_count, sizeof(CELL *), compare_xy);

		int prev_best_length = data.best_length;
		int prev_best_start_x = data.best_start_x;
		int prev_best_start_y = data.best_start_y;
		int *prev_best_path = malloc(data.total_nodes * sizeof(int));
		memcpy(prev_best_path, data.best_path_nodes, data.total_nodes * sizeof(int));

		data.best_length = 0;
		data.best_start_x = data.best_start_y = INT_MAX;

		for (int i = 0; i < cells_count; i++) {
			if (data.best_length == data.component_size) break;
			int start_node = cells[i] - data.nodes;
			memset(data.visited, 0, data.total_nodes * sizeof(int));
			DFS(&data, start_node, 0, c);
		}

		//check if we need to update the best path
		if (data.best_length == data.component_size) {
			int update = 0;
			if (data.component_size > final_best_size) {
				update = 1;
			} else if (data.component_size == final_best_size) {
				if (data.best_start_x < best_start[0] ||
					(data.best_start_x == best_start[0] && data.best_start_y < best_start[1])) {
					update = 1;
				}
			}

			// If a better path is found, update the best path
			if (update) {
				final_best_size = data.component_size;
				best_start[0] = data.best_start_x;
				best_start[1] = data.best_start_y;
				if (global_best_path) free(global_best_path);
				global_best_path = malloc(data.component_size * sizeof(int));
				memcpy(global_best_path, data.best_path_nodes, data.component_size * sizeof(int));
			}
		}

		data.best_length = prev_best_length;
		data.best_start_x = prev_best_start_x;
		data.best_start_y = prev_best_start_y;
		memcpy(data.best_path_nodes, prev_best_path, data.total_nodes * sizeof(int));
		free(prev_best_path);
		free(cells);
	}

	// for debugging
	//print_matrix(matrix, lines, columns, output);


	//Output the result
	if (final_best_size == -1) {
		fprintf(output, "-1\n");
		//printf("-1\n");
	} else {
		fprintf(output, "%d\n", final_best_size - 1);
		//printf("%d\n", final_best_size - 1);
		for (int i = 0; i < final_best_size; i++) {
			int index = global_best_path[i];

			fprintf(output, "(%d,%d)", data.nodes[index].x, data.nodes[index].y);
			//printf("(%d,%d)", data.nodes[index].x, data.nodes[index].y);

			if(final_best_size > i+1){
				fprintf(output, " ");
				//printf(" ");
			} else {
				fprintf(output, "\n");
				//printf("\n");
			}
			
		}
	}

	for (int i = 0; i < data.total_nodes; i++){
		free(data.nodes[i].neighbors);
	}

	free(data.nodes);
	free(data.comp_of);
	free(data.visited);
	free(data.path);
	free(data.best_path_nodes);
	if (global_best_path){
		free(global_best_path);
	}
}