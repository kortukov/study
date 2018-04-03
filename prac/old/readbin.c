#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if (argc < 2){
		fprintf(stderr, "Usage: ./prog FILE\n");
		return 1;
	}
	int *p;
	p = (int*) malloc(sizeof(int));
	FILE* F = fopen(argv[1], "rb");
	if (F == NULL){
		fprintf(stderr, "Error opening file\n");
		return 1;
	}
	while(!feof(F)){
		fread(p,sizeof(int), 1, F);
		printf("%d ", *p);
	}
	putchar('\n');
	free(p);
	fclose(F);
	return 0;
}
