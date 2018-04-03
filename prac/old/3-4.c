#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char str[3] = {'a', 'b', 'c'}, str2[6] = {'0','0','0','0','0','0'};
	int fd[2], i;
	if ((pipe(fd)) < 0){
		perror("error with pipe\n");
		return 1;
	}
	write(fd[1], str, 3);
	read(fd[0], str2, 6);
	for(i = 0; i < 6; i++){
		write(1, &str2[i], 1);	
		}
	return 0;
}
/*Судя по всему read читает столько символов, сколько есть
 * в канале, после чего заканчивает работать*/
