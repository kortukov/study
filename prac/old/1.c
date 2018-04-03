#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main (int argc, char** argv) {
	
	int fd, i, j, k, len, inf, rez, wstatus;
	pid_t pid;
	char com[10][80];
	char *arr[80];
	char str[80], c, result[100]= "";
	char t = '\t';
	char err[11] = "EXEC_ERROR";
	if (argc < 2){
		perror("Error: no file");
		return 1;
	}
	if((fd = open(argv[1], O_RDONLY)) < 0){
		perror("Error with file");
		return 1;
	}

	for (i = 0; i < 10; i++){
		j = 0;
		len = 0;
		c = '1';
		while (c != '\n'){
			if (read(fd, &c, 1) <= 0)
				break;
			str[len++] = c;
		}
		if(len == 0){
			break;
		}
		str[len-1] = '\0';
		strcpy(com[i], str);
	}
	
	
	for (j = 0; j<i; j++){
		puts("The string is:");
		puts(com[j]);
	}
	

	if ((rez = open("rez", O_RDWR | O_CREAT, 0666)) < 0){
		perror("Error with file rez");
		return 1;
	}
	if ((inf = open("info", O_RDWR | O_CREAT, 0666)) < 0){
		perror("Error with file info");
		return 1;
	}
	
	for( j = 0; j<i; j++){
	
		if ((pid = fork()) <0){
			close(fd);
			close(inf);
			close(rez);
			perror("Error");
			return 1;
		}
		if (pid == 0){
	
			if ((pid = fork()) <0){
				close(fd);
				close(inf);
				close(rez);
				perror("Error");
				return 1;
			}
			
			if(pid == 0){
				dup2(rez, 1);	
				/*разбиение*/
				k = 0;
				arr[k++] = strtok(com[j], " ");
				while ((arr[k++] = strtok(NULL, " ")) != NULL);
				/*разбиение*/
				if (execvp(arr[0], arr) == -1){
					perror("error exec");
				}
				exit(-103);
			}			
			wait(&wstatus);
			strcpy(result, "");
			if (WIFEXITED(wstatus)){
				strcat(result, com[j]);
				strcat(result, "\t");
		
				if(!WEXITSTATUS(wstatus))
					strcat(result, "OK");
				else if (WEXITSTATUS(wstatus) == -103){
					strcat(result, "EXEC_ERROR");
				}
				else {
					strcat(result, "ERROR");
				}
				strcat(result, "\n"); 
				/*strcpy(result, "Okay debug\n");*/
				puts(result);
				write(inf, &result, strlen(result));
			}
			close(fd);
			close(rez);
			close(inf);
			exit(0);
		} 
	}
	while (wait(NULL) > 0);
	close(fd);
	close(rez);
	close(inf);
	return 0;
}
			
				
