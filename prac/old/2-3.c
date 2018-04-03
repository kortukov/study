#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 4
int main(int argc, char** argv)
{
	FILE *F1, *F2;
	char *str;
	int number, bufsize = SIZE, length;
	long  index;
	
	if (argc < 3){
		fprintf(stderr, "Usage: ./prog file <>= number");
		return 1;
	}
	if (!(F1 = fopen(argv[1], "r"))){
		fprintf(stderr,"Error opening file");
		return 1;
	}
	if (!(F2 = fopen("tmp.txt", "w"))){
		fprintf(stderr,"Error opening file");
		return 1;
	}
	number = atoi(argv[2]);
	
	
	if (!(str = (char*)malloc(bufsize))) return 1;
	
	while (!feof(F1)){
		index = ftell(F1);
		if (!(fgets(str, bufsize, F1))) break; /*выйти если ничего не считалось*/
		while( (ftell(F1) + 1 - index) == bufsize){
			bufsize *= 2;
			free(str);
			if (!(str = (char*)malloc(bufsize))) return 1;
			fseek(F1, index, SEEK_SET);
			fgets(str, bufsize, F1);
		}
		length = strlen(str);
		if (length <= number){
			fputs(str, F2);
		}
		bufsize = SIZE;
	}
	if(remove(argv[1])) return 1;
	if(rename("tmp.txt",argv[1])) return 1;
	free(str);
	fclose(F1);
	fclose(F2);
	return 0;
}


