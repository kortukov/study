#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/* Общая логика работы программы: Считаю количество файлов и количество флагов, пытаясь открыть файлы.
 * Прохожу циклом по файлам, выполняя в одной функции filesort сразу и сортировку и вывод. Читаю файл в память и там выполняю с ним операции.
 * Написал две отдельные функции для цифровой сортировки и лексикогрфической, оба алгоритма - quicksort.  */
char* ReadStr(int );
char** file_read(int, int, char**);
int filesort(char**, int, int, int , int, int);
int file_string_count(int);
void int_sort(int*, int*, int, int, int);
int int_partition(int*, int*,  int, int, int, int);
void string_sort(char**, int, int, int);
int string_partition(char**, int, int, char*, int);

int main (int argc, char** argv)
{
	int i, j, filed[10], num_of_params = 0, num_of_files = 0, num_of_strings = 0;
	/*specifiers: boolean, boolean, starting string, output filedescriptor*/
	int reverse = 0, numeric = 0, startwithn = 0, output = 1;
	int specerror = 0;
	char **Strings;	
	if (argc > 15 || argc < 2){
		fprintf(stderr, "I can sort from 1 to 10 files at once\n");
		return 1;
	}
	for (i = 0; i < argc-1; i++)
	{
		if ((filed[i] = open(argv[argc-i-1], O_RDONLY)) < 0){
			num_of_params = argc-i-1;
			num_of_files = i;
			break;
		}
		else{
			num_of_files++;
			num_of_params = argc - num_of_files - 1;
		}
		
	}
	if (num_of_files <= 0){
		fprintf(stderr, "Couldn't open any files\n");
		return 1;
	}	
	/*specifiers*/
	for (i = 1; i <= num_of_params; i++){
		if (strcmp(argv[i],"-r") == 0){
			reverse = 1;
			continue;
		}
		if (strcmp(argv[i],"-n") == 0){
			numeric = 1;
			continue;
		}
		if (argv[i][0] == '+'){
			if((startwithn = atoi(argv[i]+1)) == 0){
				fprintf(stderr, "Arguments: -r -n +number -o\nAborting\n");
				specerror = 1;
				break;
			}
			continue;
		}
		if (strcmp(argv[i],"-o") == 0){
			if ((filed[num_of_files -1] = open(argv[argc - num_of_files], O_RDWR | O_CREAT, 0666)) < 0){
				perror("Error opening output file\n");
				specerror = 1;
				break;
			}
			output = filed[num_of_files - 1];
			num_of_files--;
			if (num_of_files <= 0){
				perror("Specify input files\n");
				specerror = 1;
				break;
			}
			continue;
		}
		else{
			fprintf(stderr, "Arguments: -r -n +n -o\nAborting\n");
			specerror = 1;
			break;
		}
	}
	if(specerror){
		for (i = 0; i < num_of_files; i++){
			close(filed[i]);
		}
		return 1;
	}
	
	/*Цикл по файлам*/
	for (i = 0; i < num_of_files; i++){
		num_of_strings = file_string_count(filed[i]);
		if (!(Strings = malloc(num_of_strings*sizeof(char*)))){
			perror("Memory error");
			break;
		}
		if (file_read(filed[i], num_of_strings, Strings) == NULL ){ /*считали файл в Strings*/
			for (j = 0; j < num_of_strings; j++)
				free(Strings[j]);
			free(Strings);
			break;
		}
		
		filesort(Strings, num_of_strings, reverse, numeric, startwithn, output);

		for (j = 0; j < num_of_strings; j++)
			free(Strings[j]);
		free(Strings);
	}
	for (i = 0; i < num_of_files; i++){
		close(filed[i]);
	}
	return 0;
}


int filesort(char **Strings, int num_of_strings, int reverse, int numeric, int startwithn, int output)
{
	int i;
	char slashn = '\n';
	if(numeric){
		int *Arr, *Ind;
		if (!(Arr = (int*)malloc(sizeof(int)*num_of_strings))){
			perror("memory error filesort"); 
			return 1;
		}
		if (!(Ind = (int*)malloc(sizeof(int)*num_of_strings))){
			free(Arr);
			perror("memory error filesort"); 
			return 1;
		}
		for(i = 0; i < num_of_strings; i++){
			Arr[i] = atoi(Strings[i]);
			Ind[i] = i;
		}
		int_sort(Arr, Ind, startwithn, num_of_strings -1, reverse);
		for(i = 0; i < num_of_strings; i++){
			write(output, Strings[Ind[i]], strlen(Strings[Ind[i]]));
			write(output, &slashn, 1);
		}
		free(Arr);
		free(Ind);
	}
	else{
		string_sort(Strings, startwithn, num_of_strings -1, reverse);
	
		for(i = 0; i < num_of_strings; i++){
			write(output, Strings[i], strlen(Strings[i]));
			write(output, &slashn, 1);
		}
	}
	return 0;
}
void int_sort(int* Arr, int* Ind, int left, int right, int reverse)
{
	int index;
	int pivot;
	if (left >= right)
			return;
		pivot = Arr[(left + right)/2];
		index = int_partition(Arr, Ind, left, right, pivot, reverse);
		int_sort(Arr, Ind, left, index - 1, reverse);
		int_sort(Arr, Ind, index, right, reverse);
}

int int_partition(int* Arr, int* Ind, int left, int right, int pivot, int reverse)
{
	int buf, bufind;
	while(left <= right){
		if(!reverse){
			while(Arr[left] < pivot)
			left++;
			while(Arr[right] > pivot)
			right--;
		}
		else if (reverse){
			while(Arr[left] > pivot)
			left++;
			while(Arr[right] < pivot)
			right--;
		}
		if (left <= right){
			buf = Arr[left];
			bufind = Ind[left];
			Arr[left] = Arr[right];
			Ind[left] = Ind[right];
			Arr[right] = buf;
			Ind[right] = bufind;
			
			left++;
			right--;
		}
	}
	return left;
}

void string_sort(char** Strings, int left, int right, int reverse)
{
	int index;
	char* pivot;
	if (left >= right)
			return;
		pivot = Strings[(left + right)/2];
		index = string_partition(Strings, left, right, pivot, reverse);
		string_sort(Strings, left, index - 1, reverse);
		string_sort(Strings, index, right, reverse);
}

int string_partition(char** Strings, int left, int right, char* pivot, int reverse)
{
	char *buf;
	while(left <= right){
		if(!reverse){
			while(strcmp(Strings[left],pivot) < 0)
			left++;
			while(strcmp(Strings[right],pivot) > 0)
			right--;
		}
		else if (reverse){
			while(strcmp(Strings[left],pivot) > 0)
			left++;
			while(strcmp(Strings[right],pivot) < 0)
			right--;
		}
		if (left <= right){
			buf = Strings[left];
			Strings[left] = Strings[right];
			Strings[right] = buf;
			left++;
			right--;
		}
	}
	return left;
}
int file_string_count(int fd)
{
	char c, cpred, check = -1;
	int i = 0;
	while( (check = read(fd, &c, 1)) ){
		if (c == '\n')
			i++;
			cpred = c;
	}
	if ( cpred != '\n')
		i++;
	lseek(fd, 0, SEEK_SET);	
	return i;
}

char** file_read(int fd, int num, char** Strings)
{
	int i;
	lseek(fd, 0, SEEK_SET);
	
	for (i = 0; i < num; i++){
		Strings[i] = ReadStr(fd);
		if(Strings[i] == NULL){
			return NULL;
		}
	}
	lseek(fd, 0, SEEK_SET);
	return Strings;
	
}

char* ReadStr(int fd)
{	
	int symbolsread = 1;
	char *strp, c, *checkp;
	int counter = 0, bufcount = 0, numbuf = 1;
	if (!(strp = malloc(8))) return NULL;

	if ((symbolsread = (read(fd, &c, sizeof(char)))) < 0){
		perror("error reading");
		return NULL;
	}
	
	while ((c != '\n')&&(symbolsread == 1)){
		bufcount++;
		strp[counter++] = c;
		if (bufcount > 7) {
			numbuf++;
			checkp = realloc(strp, 8*numbuf);  
			if (!checkp) return NULL;
			else strp = checkp;
			bufcount = 0;
		}
		if ((symbolsread = (read(fd, &c, 1))) < 0) {perror("error reading"); return NULL;}
	}
	strp[counter] = '\0';
	return strp;
}
