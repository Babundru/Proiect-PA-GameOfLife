#include "generations.h"

void push(GEN** gen, int number){
	GEN* newGen = malloc(sizeof(GEN));
	newGen->number = number;
	newGen->pos = NULL;
	newGen->next = *gen;
	*gen = newGen;
}

void printGen(GEN* gen, FILE *output){
	//while (gen != NULL) {
		fprintf(output, "%d", gen->number);
		print_coords(gen->pos, output);
		//gen = gen->next;
		fprintf(output, "\n");
	//}
}
