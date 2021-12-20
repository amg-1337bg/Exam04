#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CD_BADARG "error: cd: bad arguments\n"
#define CD_ERROR "error: cd: cannot change directory to "

typedef	struct s_pipes
{
	char *args[500];
}				t_pipe;

typedef struct s_data
{
	char *args[500];
	t_pipe pipes[500];
}				t_data;

int		ft_strlen(char *s)
{
	int i = -1;
	while (s[++i]);
	return (i);
}

void	cd(t_data cmd)
{
	if (cmd.args[1] == 0)
		write(2, CD_BADARG, ft_strlen(CD_BADARG));
	else if (chdir(cmd.args[1]) != 0)
	{
		write(2, CD_ERROR, ft_strlen(CD_ERROR));
		write(2, cmd.args[1], ft_strlen(cmd.args[1]));
		write(2, "\n", ft_strlen(cmd.args[1]));
	}
}


void	execute(t_data cmd, char **envp, int p)
{
	int fd[2];
	int pid[500], i = 0, infd = 0;

	if (strcmp("cd", cmd.args[0]) == 0)
	{
		cd(cmd);
		return ;
	}
	if (p == 0)
	{
		pid[0] = fork();
		if (pid[0] == 0)
			execve(cmd.args[0], cmd.args, envp);
		else
			waitpid(pid[0], 0, 0);
		return ;
	}
	while(i <= p)
	{
		pipe(fd);
		if (i == 0)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				dup2(fd[1], 1);
				close(fd[0]);
				if (execve(cmd.args[i], cmd.args, envp) < 0)
					exit (1);
			}
		}
		else if (i == p)
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				dup2(infd, 0);
				close(fd[1]);
				if (execve(cmd.pipes[i - 1].args[0], cmd.pipes[i - 1].args, envp) < 0)
					exit (2);
			}
		}
		else
		{
			pid[i] = fork();
			if (pid[i] == 0)
			{
				dup2(fd[1], 1);
				dup2(infd, 0);
				if (execve(cmd.pipes[i - 1].args[0], cmd.pipes[i - 1].args, envp) < 0)
					exit (3);
			}
		}
		close(infd);
		infd = dup(fd[0]);
		close(fd[1]);
		close(fd[0]);
		i++;
	}
	i = -1;
	while(++i < p)
	{
		waitpid(pid[i], 0, 0);
	}
	close(fd[1]);
	close(fd[0]);
}

int main(int ac, char **av, char **envp)
{
	if (ac < 2)
		return (1);
	av++;
	t_data cmd;
	int	i = 0, p = 0;
	while (*av)
	{
		// printf("cmd = %s\n", *av);
		if (strcmp("|", *av) == 0)
		{
			// pipe
			int pi = 0;
			av++;

			while (*av)
			{
				if (strcmp("|", *av) == 0 || strcmp(";", *av) == 0)
				{
					av--;
					p++;
					break ;
				}
				cmd.pipes[p].args[pi] = *av;
				cmd.pipes[p].args[++pi] = 0;
				if (*(av + 1) == NULL)
					p++;
				av++;
			}
		}
		else if (strcmp(";", *av) == 0)
		{
			execute(cmd, envp, p);
			p = 0;
			i = -1;
		}
		else
			cmd.args[i] = *av;
		av++;
		i++;
		cmd.args[i] = 0;
	}
	execute(cmd, envp, p);
	// i = -1;
	// while(cmd.args[++i])
	// 	printf("%s\n", cmd.args[i]);
	// int pp = -1;
	// while (++pp < p)
	// {
	// 	int ii = -1;
	// 	while (cmd.pipes[pp].args[++ii])
	// 		printf("pipe ===> %s\n", cmd.pipes[pp].args[ii]);
	// }
}