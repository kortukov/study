#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	int first, last, end;
	int pipe12[2], pipe23[2], pipe32[2], pipe21[2];
	pid_t pid2, pid3;
	if (argc < 3){
		perror("Specify two integers");
		return 1;
	}
	first = atoi(argv[1]);
	last = atoi(argv[2]);
	end = first - 1;
	
	if (pipe(pipe12) < 0){
		perror("Error opening pipe1\n");
		return 1;
	}
	if (pipe(pipe23) < 0){
		perror("Error opening pipe1\n");
		return 1;
	}
	if (pipe(pipe32) < 0){
		perror("Error opening pipe1\n");
		return 1;
	}
	if (pipe(pipe21) < 0){
		perror("Error opening pipe1\n");
		return 1;
	}
	
	pid2 = fork();
	if (pid2 < 0){
		perror("Error forking processes\n");
		return 1;
	}
	/*Son process*/
	if (pid2 == 0){
		pid3 = fork();
		if (pid3 < 0){
			perror("Error forking processes\n");
			return 1;
		}
		/*Grandson process*/
		if (pid3 == 0){
			close(pipe12[0]);
			close(pipe12[1]);
			close(pipe21[0]);
			close(pipe21[1]);
			close(pipe32[0]);
			close(pipe23[1]);
			
			for(;;){
				read(pipe23[0], &first, sizeof(int));
				printf("Grandson Number: %d My pid is: %d\n", first, getpid());
				if( first == end)
					break;
				if (first > last){
					write(pipe12[1], &end, sizeof(int));
					break;
				}
				first++;
				write(pipe32[1], &first, sizeof(int));
			}	
			close(pipe32[1]);
			close(pipe23[0]);
			return 0;
		}
		/*Son process starts here*/
		close(pipe23[0]);
		close(pipe32[1]);
		close(pipe12[1]);
		close(pipe21[0]);
		
		for(;;){
			read(pipe12[0], &first, sizeof(int));
			printf("Son Number: %d My pid is: %d\n", first, getpid());
			if( first == end){
				write(pipe23[1], &end, sizeof(int));
				break;
			}
			if (first > last){
				write(pipe23[1], &end, sizeof(int));
				write(pipe21[1], &end, sizeof(int));
				break;
			}
			first++;
			write(pipe23[1], &first, sizeof(int));
			read(pipe32[0], &first, sizeof(int));
			printf("Son Number: %d My pid is: %d\n", first, getpid());
			if( first == end){
				write(pipe21[1], &end, sizeof(int));
				break;
			}
			if (first > last){
				write(pipe23[1], &end, sizeof(int));
				write(pipe21[1], &end, sizeof(int));
				break;
			}
			first++;
			write(pipe21[1], &first, sizeof(int));
		}
		
		close(pipe23[1]);
		close(pipe32[0]);
		close(pipe12[0]);
		close(pipe21[1]);
		return 0;
	}
	/*fatherprocess*/
	close(pipe23[0]);
	close(pipe23[1]);
	close(pipe32[0]);
	close(pipe32[1]);
	close(pipe12[0]);
	close(pipe21[1]);
	
	printf("Father Number: %d My pid is: %d\n", first, getpid());
	write(pipe12[1], &first, sizeof(int));
	for(;;){
		read(pipe21[0], &first, sizeof(int));
		printf("Father Number: %d My pid is: %d\n", first, getpid());
		if( first == end)
			break;
		if (first > last){
			write(pipe12[1], &end, sizeof(int));
			break;
		}
		first++;
		write(pipe12[1], &first, sizeof(int));
	}	
	close(pipe12[1]);
	close(pipe21[0]);
	return 0;
}
