#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 30

volatile int main(int argc, char *argv[], char **env)
{
	int	idx_fork = 0;
	int	num_fork = 2;
	int fd1[2][2];
	char buffer[BUFSIZE];
	pid_t pid;

	while (idx_fork < num_fork)
	{
		if(pipe(fd1[idx_fork]) == -1)
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
		else if (pid != 0)
			break ;
		idx_fork++;
	}
	if (idx_fork == 0)
	{
		int		state;
		printf("parent Wating...\n");
		waitpid(pid, &state, WNOHANG);
		printf("parent END Wating...\n");
		if (WEXITSTATUS(state) != 0)
			exit(WEXITSTATUS(state));
		else
			exit(0);
	}
	else if (idx_fork == 1)
	{
		char	**cat;
		int		state;
		int fd = open("./out1", O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
			printf("ERR1");
		cat = (char **)malloc(sizeof(char *) * 3);
		cat[0] = strdup("/bin/cat");
		cat[1] = strdup("-n");
		cat[2] = NULL;
		printf("WAITING...\n");
		waitpid(pid, &state, 0);
		printf("END WAITING, %d\n", fd);
		if (WEXITSTATUS(state) != 0)
			exit(WEXITSTATUS(state));
		close(fd1[idx_fork][1]);
		dup2(fd1[idx_fork][0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		execve(cat[0], cat, env);
	}
	else
	{
		char	**cat;
		int fd = open("./redirection_Test.c", O_RDONLY);
		int fd2 = open("./test", O_RDONLY);
		if (fd < 0)
			printf("ERR2");
		cat = (char **)malloc(sizeof(char *) * 3);
		cat[0] = strdup("/bin/cat");
		cat[1] = strdup("./aa");
		cat[2] = NULL;
		printf("%d : %d, %d\n", idx_fork, fd1[idx_fork - 1][1], fd);
		dup2(fd1[idx_fork - 1][1], STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		dup2(fd2, STDIN_FILENO);
		execve(cat[0], cat, env);
	}
	return 0;
}
