#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 16
void Merge(int* A, int m, int* B, int n, int* C);

int main( int argc, char** argv )
{
	
	int *A, *B, *C;
	int i;
	
	if ( argc < 3 ) {
		fprintf( stderr, "Error" );
		return 1;
	};
	
	FILE *f1 = fopen( argv[1], "rb" );
	if ( f1 == NULL ) {
		fprintf( stderr, "Error" );
		return 1;
	};
	
	FILE *f2 = fopen( argv[2], "rb" );
	if ( f2 == NULL ) {
		fprintf( stderr, "Error" );
		return 1;
	};
	
	FILE *f = fopen( "filefile.bin", "wb" );
	if ( f == NULL ) {
		fprintf( stderr, "Error" );
		return 1;
	};
	
	A = (int*)malloc(BUFSIZE*sizeof(int));
	B = (int*)malloc(BUFSIZE*sizeof(int));
	
	if ((A == NULL) || (B == NULL)){
		fprintf(stderr, "Error allocating memory");
		return 0;
	}
	
	int bufsize1 = BUFSIZE;
	int counter1 = 0;
	
	while(!feof(f1)){
		while (counter1 < bufsize1){
			fread(&A[counter1], sizeof(int), 1, f1);
			if(feof(f1))break;
			counter1++;
		}
		if (feof(f1)) break;
		bufsize1 *= 2;
		int* checkp;
		checkp = (int*)realloc(A, bufsize1*sizeof(int));
		if(checkp == NULL) return 1;
		A = checkp;
	}
	
	int bufsize2 = BUFSIZE;
	int counter2 = 0;
	
	while(!feof(f2)){
		while (counter2 < bufsize2){
			fread(&B[counter2], sizeof(int), 1, f2);
			if(feof(f2))break;
			counter2++;
		}
		if (feof(f2)) break;
		bufsize2 *= 2;
		int* checkp;
		checkp = (int*)realloc(B, bufsize2*sizeof(int));
		if(checkp == NULL) return 1;
		B = checkp;
	}
	
	C = (int*)malloc((bufsize1+bufsize2)*sizeof(int));
	Merge(A,counter1,B,counter2,C);
	
	fwrite(C, sizeof(int), counter1+counter2, f);
	
	fclose(f1);
	fclose(f2);
	fclose(f);
	
	return 0;
}



void Merge(int* A, int m, int* B, int n, int* C)
{	
	int it1 = 0;
	int it2 = 0;
	int* Buf = (int*)malloc((m+n)*sizeof(int));
	if (Buf == NULL) exit(1);
	int i;	
	
	while((it1 < m) && (it2 < n)){
		if (A[it1] < B[it2]){
			Buf[it1+it2] = A[it1];
			it1++;
		}
		else{
			Buf[it1+it2] = B[it2];
			it2++;
		}
	}
	while (it1 < m){
		Buf[it1+it2] = A[it1];
		it1++;
	}
	while (it2 < n){
		Buf[it1+it2] = B[it2];
		it2++;
	}
	
	for(i = 0; i < m+n; i++)
		C[i]=Buf[i];
	
	free(Buf);
}
