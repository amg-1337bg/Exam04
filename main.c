#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef	struct s_pipes
{
	char *args[500];
}				t_pipe;

typedef struct s_data
{
	char *args[500];
	t_pipe pipes[500];
}				t_data;

int main(int ac, char **av, char **envp)
{
	if (ac < 2)
		return (1);
	av++;
	t_data cmd;
	int	i = 0, p = 0;
	while (*av)
	{
		printf("cmd = %s\n", *av);
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

		}
		else
		{
			cmd.args[i] = *av;
			cmd.args[++i] = 0;
		}
		av++; 
	}
	i = -1;
	while(cmd.args[++i])
		printf("%s\n", cmd.args[i]);
	int pp = -1;
	while (++pp < p)
	{
		int ii = -1;
		while (cmd.pipes[pp].args[++ii])
			printf("pipe ===> %s\n", cmd.pipes[pp].args[ii]);
	}
}