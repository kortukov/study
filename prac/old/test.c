#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Node {
    char *Value;
    struct _Node *next;
    struct _Node *prev; 
} Node;

typedef struct _List {
    int size;
    Node *head;
    Node *tail;
} List;
	
List* createList( void ) {
    List *tmp = (List*) malloc(sizeof(List));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
 
    return tmp;
};

void delList(List **list) {
    Node *tmp = (*list)->head;
    Node *next = NULL;
    while (tmp) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    };
    free(*list);
    (*list) = NULL;
};
	
char* ReadStr(void)
{
	char* strp, c, *checkp;
	int count = 0, bufcount = 0, numbuf = 1;
	if (!(strp = malloc(8))) return NULL;
	while ((c = getchar()) != '\n') {
		bufcount++;
		strp[count++] = c;
		if (bufcount > 7) {
			numbuf++;
			checkp = realloc(strp, 8*numbuf);  
			if ( !checkp ) 
			return NULL;
			else strp = checkp;
			bufcount = 0;
		}
	}
	return strp;
};

void pushFront(List *list, char *str) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->Value = str;
    tmp->next = list->head;
    tmp->prev = NULL;
    if (list->head) {
        list->head->prev = tmp;
    };
    list->head = tmp;
 
    if (list->tail == NULL) {
        list->tail = tmp;
    };
    list->size++;
};

void printVal(char *value) {
    printf("%s \t ", value);
};
				
void printList(List *list, void (*printVal)(char*)) {
    Node *tmp = list->head;
    while (tmp) {
        printVal(tmp->Value);
        tmp = tmp->next;
    }
    printf("\n");
};

Node* getNode(List *list, int index) {
    Node *tmp = list->head;
    int i = 0;
 
    while ((tmp) && (i < index)) {
        tmp = tmp->next;
        i++;
    }
 
    return tmp;
};

void* delNode(List *list, int index) {
    Node *elm = NULL;
    char *tmp = NULL;
    elm = getNode(list, index);
    if (elm == NULL) {
        exit(1);
    };
    if (elm->prev) {
        elm->prev->next = elm->next;
    }
    if (elm->next) {
        elm->next->prev = elm->prev;
    }
    tmp = elm->Value;
 
    if (!elm->prev) {
        list->head = elm->next;
    }
    if (!elm->next) {
        list->tail = elm->prev;
    }
 
    free(elm);
 
    list->size--;
 
    return tmp;
};


List* Func( List* list1, List* list2) {
	
	Node *tmp1 = list1->head;
	Node *tmp2 = list1->head;
	int i=0, j=0;
	
	if ((list1->head->next == NULL) && (list2->head->next == NULL)) {
		return NULL;
	}
	else if (list1->head->next == NULL) {
		return (list2);
	}
	else if (list2->head->next == NULL) {
		return (list1);
	};
	
	
	do{	i++; 
		
		do { j++;
			if (!strcmp( tmp1->Value, tmp2->Value )) {
			delNode(tmp1, i);
			i--;
			delNode(tmp2, j);
			j--;
		} else tmp2 = tmp2->next;	
		
	}	while ( (tmp2->next) != NULL ) ;
		tmp1 = tmp1->next;
			
	}	while ( (tmp1->next) != NULL  ) ;
	
	tmp1 = list1->head;
	tmp2 = list2->head;	
			
		do { pushFront( tmp2, tmp1->Value );
			tmp1=tmp1->next;
		} while ( (tmp1->next) != NULL  ) ;
		
	return( tmp2 );
}
			
	
	
	


int main()
{
	List* str1;
	List* str2;
	List* str;
	
	void(*funcpointer)(char*) = NULL;
	funcpointer=printVal;
	
	int n, m , i;
	char* tmp;
	
	str1 = createList();
	str2 = createList();
	str = createList();
	
	puts("Enter number of elements in L1 and L2");
	scanf("%d%d\n", &n, &m);
	
	for (i=1;i<=n;i++) {
		tmp = ReadStr();
		pushFront( str1, tmp);
		};
	for (i=1;i<=m;i++) {
		tmp = ReadStr();
		pushFront( str2, tmp);
		};	
	
	str = Func( str1, str2 );
	
	if (str == NULL) {
		printf("Error\n");
		return 1;
	};
	printList(str, funcpointer );
	return 0;
};
