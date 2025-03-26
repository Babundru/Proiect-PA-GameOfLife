#include <stdio.h>


int main(int argc, const char *argv[]){

  FILE * input, *output;
  
  for (int i = 0; i < argc; i++) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  input=fopen(argv[1], "rt");
  output=fopen(argv[2], "wt");

  fprintf(output, "Hello");

  fclose(input);
  fclose(output);
}
