#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 30

volatile int main(int argc, char *argv[], char **env)
{
	int fd1[2], fd2[2];
	char buffer[BUFSIZE];
	pid_t pid;
	if(pipe(fd1) == -1||pipe(fd2) == -1)
	{
		puts("pipe() error");
		exit(1);
	}
	pid = fork();
	if(pid == -1)
	{
		puts("fork() error");
		exit(1);
	}
	else if(pid != 0)
	{
		char	buffer;
		int		state;
		int		ret;
		int fd = open("./redirection_Test.c", O_RDONLY);
		//dup2(fd1[1], fd);
		//dup2(fd, STDIN_FILENO);
		dup2(fd1[1], STDOUT_FILENO);
		while (read(fd, &buffer, 1) == 1)
			printf("%c", buffer);
		ret = waitpid(pid, &state, 0);
		// if (WEXITSTATUS(state) != 0)
		// 	exit(WEXITSTATUS(state));
		// else
		// 	exit(0);
	}
	else
	{
		char	buff[BUFSIZE];
		char	**cat;
		int fd_file1;
		fd_file1 = open("./out1", O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd_file1 < 0)
			printf("err\n");
		else
		{
			printf("ss\n");
		}
		printf("1111");
		cat = (char **)malloc(sizeof(char *) * 3);
		printf("1111");
		cat[0] = strdup("/bin/cat");
		printf("1111");
		cat[1] = strdup("-n");
		printf("1111");
		cat[2] = NULL;
		printf("1111");
		sleep(1);
		printf("2222");
		dup2(fd1[0], STDIN_FILENO);
		printf("3333");
		dup2(fd_file1, STDOUT_FILENO);
		printf("4444");
		close(fd1[1]);
		printf("5555");
		read(STDIN_FILENO, buff, BUFSIZE);
		printf("6666");
		write(STDOUT_FILENO, buff, BUFSIZE);
		
		//printf("%s", buff);
		//printf("dsafdsafdasf");
		//execve(cat[0], cat, NULL);
		exit(1);
	}

	// int fd = open("./out1", O_WRONLY | O_TRUNC | O_CREAT);
	// if (fd < 0)
	// 	printf("err\n");
	// {
	// 	dup2(fd, STDOUT_FILENO);
	// 	printf("tetsdtdstdstsdtsd");
	// 	printf("hiiiiiii");
	// }
	return 0;
}
