#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

char* ReadStr(int );
int file_string_count(int);

int main(int argc, char** argv)
{
	int file,tmpfile, pipe1[2]/*father to son*/, pipe2[2],/*son to father*/ string_num, string_len;
	int i, j;
	char* string, c;
	pid_t pid;
	
	if (argc < 2){
		perror("Enter a file\n");
		return 1;
	}
	if ((file = open(argv[1], O_RDWR)) < 0){
		perror("Error opening file\n");
		return 1;
	}
	if ((tmpfile = open("TempFile", O_RDWR | O_CREAT, 0666)) < 0){
		perror("Error opening file\n");
		return 1;
	}
	if (pipe(pipe1) < 0){
		perror("Error opening pipe1\n");
		return 1;
	}
	if (pipe(pipe2) < 0){
		perror("Error opening pipe2\n");
		return 1;
	}
	string_num = file_string_count(file);
	pid = fork();
	if (pid < 0){
		perror("Error forking processes\n");
		return 1;
	}
	
	/*Son process */
	if (pid == 0){
		close(pipe1[1]);
		close(pipe2[0]);
		for (i = 0; i < string_num; i++){
			string = ReadStr(pipe1[0]);
			string_len = strlen(string);
			if(string[string_len - 1] == '\n')
				string_len --;
			for (j = 0; j < string_len/2; j++){
				c = string[j];
				string[j] = string[string_len - j - 1];
				string[string_len - j - 1] = c;
			}
			string_len++;
			write(pipe2[1], string, string_len);
			free(string);
		}
		close(pipe1[0]);
		close(pipe2[1]);
		return 0;
	}
	/*Father process*/
	close(pipe1[0]);
	close(pipe2[1]);
	for ( i = 0; i < string_num; i++){
		string = ReadStr(file);
		string_len = strlen(string);
		write(pipe1[1], string, string_len);
		string = ReadStr(pipe2[0]);
		write(tmpfile, string, string_len);
		free(string);
	}
	wait(NULL);
	close(pipe1[1]);
	close(pipe2[0]);
	remove(argv[1]);
	rename("TempFile", argv[1]);
	close(file);
	close(tmpfile);
	return 0;	
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
	strp[counter++] = '\n';
	strp[counter] = '\0';
	return strp;
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
