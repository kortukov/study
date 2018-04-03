#include <stdio.h>
#include <string.h>
#define MAXLEN 100


int main(int argc, char* argv[])
{
	FILE* F1;
	char c, str[MAXLEN];
	int flag = 0;
	
	if (argc < 2) {
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
	if (!(F1 = fopen(argv[1], "r"))){
		fprintf(stderr, "Error opening file\n");
		return 1;
	}
	puts("Enter character for search");
	scanf("%c", &c);
	while (!feof(F1)){
		if (!(fgets(str, MAXLEN, F1))) break;
		if (str[strlen(str)-2] == c) { 
			puts(str);
			flag = 1;
		}
	}
	if (!flag) puts("No such strings");
	fclose(F1);
	return 0;
}
