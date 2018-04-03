#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct List{
	char* string;
	struct List* next, *prev;
};
typedef struct List List;

char* ReadStr(void);
List* ListInit(void);
void ListFree(List*);
List* ListAddElement(List* , char*);
void ListPrint(List*);
void ListRead(List*, int);
List* ListMerge(List*, List*);
List* ListDeleteElement(List*);

int main(int argc, char* argv[])
{	
	int n, m;
	List* L1 = NULL, *L2 = NULL, *L3 = NULL, *listp; /*L1 - указатель на начало, listp - текущий узел списка */
	L1 = ListInit();	
	L2 = ListInit();
	puts("Enter number of elements in L1 and L2");
	scanf("%d%d\n", &n, &m);
	listp = L1;
	ListRead(listp, n);
	ListPrint(L1);
	listp = L2;
	ListRead(listp, m);
	ListPrint(L2);

	L3 = ListMerge(L1, L2);
	ListPrint(L3);
	ListFree(L3);
	return 0;
}

List* ListMerge(List* L1, List* L2)
{
	List* list1p, *list2p, *L3, *list3p;
	list1p = L1;
	list2p = L2;
	while (list1p != NULL){
		while (list2p != NULL){
			if (strcmp(list1p->string,list2p->string) == 0){
				if (list2p == L2){ /*если удаляем первый в списке элемент то нужно обработать это отдельно*/
					L2 = ListDeleteElement(list2p);
					list2p = L2;
				}
				else 
					list2p = ListDeleteElement(list2p);
			}
			else list2p = list2p->next;
		}
	list2p = L2;
	list1p = list1p->next;
	}
	/*Составим наш новый список из L1 и добавим в конец L2*/
	L3 = L1;
	list3p = L3;
	while (list3p->next != NULL)
		list3p = list3p->next;
	list3p->next = L2;
	if (list3p->next != NULL) list3p->next->prev = list3p->next;
	
	return L3;
}
char* ReadStr(void)
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
	strp[counter++] = '\0';
	return strp;
}

List* ListInit(void)
{
	List* listik;
	if ((listik = (List*)malloc(sizeof(List))) == NULL) return NULL;
	listik->string = NULL;
	listik->next = NULL;
	listik->prev = NULL;
	return listik;
}

void ListFree(List* list)
{
	if (list == NULL) exit(0);
	if (list->next != NULL) ListFree(list->next);
	free(list->string);
	free(list);
}

List* ListAddElement(List* list, char* string)
{
	List* buf;
	if(list->string == NULL){ /*случай если список ещё пустой*/
		list->string = string;
		return list;
	} 
	buf = list->next;
	if ((list->next = (List*)malloc(sizeof(List))) == NULL) return NULL;
	list->next->string = string;
	list->next->next = buf;
	list->next->prev = list;
	if (list->next->next != NULL) list->next->next->prev = list->next;
	return list->next;
}

List* ListDeleteElement(List* list) /*возвращает указатель на следующий после удалённого*/
{
	List* buf;
	if (list == NULL) return NULL;
	if (list->prev == NULL){ /*первый элемент*/
		if (list->next == NULL){ /*первый и он же последний*/
			free(list->string);
			free(list);
			return NULL;
		} 
		list->next->prev = NULL; /*если не первый */
		buf = list->next;
		free(list->string);
		free(list);
		return buf;
	}
	list->prev->next = list->next;
	buf = list->next;
	free(list->string);
	free(list);
	return buf;
}

void ListPrint(List* list)
{
	while(list != NULL){
		if (list->string != NULL) printf("%s\t", list->string);
		list = list->next;
	}
	putchar('\n');
}

void ListRead(List* listpointer, int num)
{
	int i;
	char* string;
	for (i = 0; i< num; i++){
		string = ReadStr();
		if (string == NULL) {
			printf("Error\n");
			exit(1);
		}
		else
			listpointer = ListAddElement(listpointer, string);
	}
}
