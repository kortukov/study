#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int filed[2], flagchan[2], file, counter = 0;
	pid_t pid;
	int str_len;
	char str[41], flagf, flags;
	if (argc < 2){
		perror("Enter a file\n");
		return 1;
	}
	file = open(argv[1], O_RDWR | O_CREAT, 0666);
	if (file < 0){
		perror("Error opening file");
		return 1;
	}
	if ((pipe(filed)) < 0){
		perror("Error opening stream");
		close(file);
		return 1;
	}
	if ((pipe(flagchan)) < 0){
		perror("Error opening stream");
		close(filed[0]);
		close(filed[1]);
		close(file);
		return 1;
	}
	pid = fork();
	if (pid < 0){
		perror("Error fork");
		close(filed[0]);
		close(filed[1]);
		close(flagchan[0]);
		close(flagchan[1]);
		close(file);
		return 1;
	}
	if (pid == 0){
		close(filed[0]);
		close(flagchan[1]);
		printf("Son process\n");
		for(;;){
			fgets(str, 40, stdin);
			
		
			if((strcmp("exit\n", str)) == 0){	
				putchar('e');		
				close(filed[1]);
				close(flagchan[0]);
				close(file);
				return 0;
				
			}
			write(filed[1], str, strlen(str));	
			read(flagchan[0], &flags, 1);
			for (counter = 0; counter < 10; counter++)
				write(file, &flags, 1);
		}		
	}
	else {
		close(filed[1]);
		close(flagchan[0]);
		printf("Father process\n");
		for(;;){
			counter = 0;
			while (((str_len = read(filed[0], &flags, 1))>0)&&(flags!='\n'))
				str[counter++] = flags;
				
			if ( str_len < 0){
					printf("Ended father");
					close(filed[0]);
					close(flagchan[1]);
					close(file);
					wait(NULL);
					return 1;
				}

			
			str[counter++] = '\n';
			str[counter] = '\0';
			write(file, str, strlen(str));
			flagf = '+';
			write(flagchan[1], &flagf, 1);
			
		}
	}
	return 0;
}
