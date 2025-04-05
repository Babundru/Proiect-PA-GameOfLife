#include "coords.h"

struct generation{
	int number;
	COORDS *pos;
	struct generation *next;
};

typedef struct generation GEN;

void push(GEN** gen, int number);
void printGen(GEN* gen, FILE *output);