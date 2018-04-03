#include <stdio.h>
#include <stdlib.h>
#define SIZE 3
int main(int argc, char* argv[])
{
	FILE* F1, *F2;
	
	char *str1, *str2;
	long int index;
	int bufsize = SIZE;
	if (argc < 2){
		fprintf(stderr, "Too few arguments");
		return 1;
	}
	if (!(F1 = fopen(argv[1], "rw"))){
		fprintf(stderr,"Error opening file");
		return 1;
	}
	if (!(F2 = fopen("tmp.txt", "w"))){
		fprintf(stderr,"Error opening file");
		return 1;
	}
	
	if (!(str1 = (char*)malloc(bufsize))) return 1;
	if (!(str2 = (char*)malloc(bufsize))) return 1;
	
	while (!feof(F1)){
		index = ftell(F1);
		if (!(fgets(str1, bufsize, F1))) break;
		while( (ftell(F1) + 1 - index) == bufsize){
			bufsize *= 2;
			free(str1);
			if (!(str1 = (char*)malloc(bufsize))) return 1;
			fseek(F1, index, SEEK_SET);
			fgets(str1, bufsize, F1);
		}
		bufsize = SIZE;
		index = ftell(F1);
		if (!(fgets(str2, bufsize, F1))) {fputs(str1, F2); break;}
		while( (ftell(F1) + 1 - index) == bufsize){
			bufsize *= 2;
			free(str2);
			if (!(str2 = (char*)malloc(bufsize))) return 1;
			fseek(F1, index, SEEK_SET);
			fgets(str2, bufsize, F1);
		}
		fputs(str2, F2);
		fputs(str1, F2);
		bufsize = SIZE;
	}
	
	if(remove(argv[1])) return 1;
	if(rename("tmp.txt",argv[1])) return 1;
	
	free(str1);
	free(str2);
	fclose(F1);
	fclose(F2);
	return 0;
}
