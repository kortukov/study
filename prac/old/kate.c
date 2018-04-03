#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 struct List {
		char* Value;
		struct List* next;
		struct List* prev;
};
typedef struct List List;

char* ReadStr( void ) {
	
	char* strp, c, *checkp;
	int count = 0, bufcount = 0, numbuf = 1;
	if (!(strp = malloc(8))) return NULL;
	while ((c = getchar()) != '\n'){
		bufcount++;
		strp[count++] = c;
		if (bufcount > 7) {
			numbuf++;
			checkp = realloc(strp, 8*numbuf);  
			if (!checkp) return NULL;
			else strp = checkp;
			bufcount = 0;
		}
	}
	strp[count++] = '\0';
	return strp;
}

List* CreateList( void ) {
	
	List* list;
	if ( (list = (List*)malloc(sizeof(List))) == NULL )
		return 0;
	list->next = NULL;
	list->prev = NULL;
	list->Value = NULL;
	
	return( list );
}

void CleanList( List* list ) {
	
	if ( list == NULL ) 
		exit(0);
	if ( list->next != NULL )
		CleanList( list->next );
	free( list->Value );
	free( list );
}

void PrintList( List* list ) {
	
	List* tmp;
	tmp = list;
	putchar('\n');
	while ( tmp != NULL ) {
		if ( tmp->Value != NULL )
			printf( "%s\n", tmp->Value );
		tmp = tmp->next;
	};
	putchar('\n');
}

List* AddElem( List* list, char* str ) {
	
	List* tmp;
	if ( list->Value == NULL ) { 
		list->Value = str;
		return list;
	} 
	tmp = list->next;
	if ( ( list->next = (List*)malloc(sizeof(List)) ) == NULL) 
		return NULL;
	list->next->Value = str;
	list->next->next = tmp;
	list->next->prev = list;
	if ( list->next->next != NULL ) 
		list->next->next->prev = list->next;
	return list->next;
}

List* DelElem ( List* list ) {
	
	List* tmp;
	if ( list == NULL ) 
		return NULL;
	if ( list->prev == NULL ){ 
		if ( list->next == NULL ){ 
			free( list->Value );
			free( list );
			return NULL;
		} 
		list->next->prev = NULL;
		tmp = list->next;
		free( list->Value );
		free( list );
		return tmp;
	}
	list->prev->next = list->next;
	tmp = list->next;
	free( list->Value );
	free( list );
	return tmp;
}


void ReadList( List* list, int n ) {
	
	int i;
	char* str;
	for ( i = 0; i < n; i++ ) {
		str = ReadStr();
		if ( str == NULL ) {
			printf( "Error \n" );
			exit(1);
		} else
			list = AddElem( list, str );
		}
	}
	
List* ComList( List* list1, List* list2 ) {
	
	List *l1, *l2, *l3, *tmp;
	int flag1 = 0;
	l1 = list1;
	l2 = list2;
	while ( l1 != NULL ) {
		
		while ( l2 != NULL ) {
			flag1 = 0;
			if ( strcmp( l1->Value, l2->Value ) == 0 ) {
				if ( (l2 == list2) && ( l1 == list1 ) ) {
					list2 = DelElem( l2 );
					l2 = list2;
					list1 = DelElem( l1 );
					l1 = list1;
					flag1 = 1;
				} else if (l2 == list2) {
					list2 = DelElem( l2 );
					l2 = list2;
					l1 = DelElem( l1 );
					flag1 = 1;
				} else if ( l1 == list1 ) {
						list1 = DelElem( l1 );
						l1 = list1;
						l2 = DelElem( l2 );
						flag1 = 1;
				} else
					l1 = DelElem( l1 );
					l2 = DelElem( l2 );
					flag1 = 1;
			} else
				l2 = l2->next;
				
		}
		l2 = list2;
		if ( flag1 == 0 )
			l1 = l1->next;
	}
	
	tmp = list1;
	l3 = tmp;
	while ( l3->next != NULL )
		l3 = l3->next;	
	l3->next = list2;
	if ( l3->next != NULL ) 
		l3->next->prev = l3;
	return ( tmp );
}

int main(int argc, char* argv[])
{
	List* list1 = NULL;
	List *list2 = NULL;
	List *list3 = NULL;
	int n, m;
	List* tmp;
	
	list1 = CreateList( );
	list2 = CreateList( );
	
	puts("Enter number of elements in lists");
	scanf("%d%d\n", &n, &m);
	
	tmp = list1;
	ReadList( tmp, n );
	tmp = list2;
	ReadList( tmp, m );
	
	PrintList( list1 );
	PrintList( list2 );
	
	list3 = ComList( list1, list2 );
	PrintList( list3 );
	CleanList( list3 );
	
	return 0;
}

