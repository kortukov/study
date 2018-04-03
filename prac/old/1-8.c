#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct List{
		char buf[8];
		int bufcount;
		struct List* next;
	};
typedef struct List List;

char* ReadStr1(void);
char* ReadStr2(void);
int PrintList(List*);
int FreeList(List*);

int main(int argc, char* argv[])
{
	char* string;
	string = ReadStr1();
	if (string == NULL) {
		printf("Error\n");
		return 1;
	}
	else{ 
		printf("%s\n", string);
		free(string);
		return 0;
	}
	return 0;
}

int FreeList(List* list)
{
	if (list == NULL) return 0;
	if (list->next) FreeList(list->next);
	free(list->next);
	return 0; 
}

char* ReadStr1(void)
{
	
	List* strlst, *listp;
	char c, *string;
	int charcount = 0, i = 0, j = 0;
	
	
	if (!(strlst = (struct List*) malloc(sizeof(struct List)))) return NULL;
	strlst->bufcount = 0;
	listp = strlst;
	while ((c = getchar()) != '\n'){
		charcount++;
		listp->buf[listp->bufcount++]=c;
		if (listp->bufcount == 8){
			if (!(listp->next = (struct List*) malloc(sizeof(struct List)))) return NULL;
			listp->next->bufcount = 0;
			listp = listp->next;
		}	
	} 
	/* now convert list to string */
	listp = strlst;
	string = (char*) malloc(charcount+1); /*sizeof(char) = 1 */
	while(listp != NULL){
		for(j = 0; j < 8; j++) 
		{
			string[i++] = listp->buf[j];
		}
		listp = listp->next;
	}
	string[i] = '\0';
	FreeList(strlst);
	return string;
}

char* ReadStr2(void)
{
	char* strp, c, *checkp;
	int counter = 0, bufcount = 0, numbuf = 1;
	if (!(strp = malloc(8))) return NULL;
	while ((c = getchar()) != '\n'){
		bufcount++;
		strp[counter++] = c;
		if (bufcount > 7) {
			numbuf++;
			checkp = realloc(strp, 8*numbuf);  
			if (!checkp) return NULL;
			else strp = checkp;
			bufcount = 0;
		}
	}
	strp[counter++] = '\n';
	return strp;
}
