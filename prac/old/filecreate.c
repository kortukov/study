#include <stdio.h>
#include <stdlib.h>

void MergeSort(int*, int, int);
void Merge(int*, int, int*, int, int*);

int main(int argc, char* argv[])
{
	int number, i, *A;
	
	if (argc < 3){
		fprintf(stderr, "Usage: ./prog FILE num_of_elements\n");
		return 1;
	}
	
	FILE* F = fopen(argv[1], "wb");
	if (F == NULL){
		fprintf(stderr, "Error opening file\n");
		return 1;
	}
	
	number = atoi(argv[2]);
	A = (int*)malloc(number*sizeof(int));
	if (A == NULL){
		fprintf(stderr, "Error with array\n");
		return 1;
	}

	srand(time(NULL));
	/*First create array*/
	for(i = 0; i < number; i++){
		A[i] = rand()%101;
	}
	
	putchar('\n');
	MergeSort(A,0,number);
	
	
	fwrite(A, sizeof(int), number, F);

	
	fclose(F);
	return 0;
}


void MergeSort(int* A, int left, int right)
{	
	if(left+1 >= right)
		return;
	int mid = (left+right)/2;
	MergeSort(A,left,mid);
	MergeSort(A,mid,right);
	Merge(A+left,mid-left,A+mid,right-mid,A+left);
	
	return;
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
