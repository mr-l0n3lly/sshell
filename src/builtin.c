#include "sshell.h"

int		is_builtin(char **args)
{
	if(strcmp(args[0], "more") == 0)
		return (1);
	if(strcmp(args[0], "cd") == 0)
		return (1);

	return (0);
};

int sshell_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "sshell: expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("sshell");
	}
	return (1);
};

int		getmoreenvi(char *name)
{
	t_env *tmp = envi;

	while(tmp != NULL)
	{
		if(strcmp(tmp->name, name) == 0)
			return (atoi(tmp->value));
		tmp = tmp->next;
	}
	return (-1);
};

void	more(FILE *fp, int v, int h)
{
	char	*line;
	size_t	len = h;
	ssize_t read;
	char	c = '\n';

	int i = 0;
	while(c == '\n')
	{
		execute_cmd("clear");
		i = 0;
		while (i < v)
		{
			if((read = getline(&line, &len, fp)) == -1)
			{
				printf("Done!\n");
				return;
			}
			printf("%s", line);
			i++;
		}
		printf(": ");
		scanf("%c", &c);
		fflush(stdin);
	}
	printf("Exited\n");
	return;
};

int		sshell_more(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "sshell: expected argument to \"more\"\n");
	}
	else
	{
		int v = getmoreenvi("VSIZE");
		int h = getmoreenvi("HSIZE");
		if (v == -1)
		{
			fprintf(stderr, "ERROR the vsize argument isnt set!\n");
			return (0);
		}
		if (h == -1)
		{
			fprintf(stderr, "ERROR the hzise argument isnt set!\n");
			return (0);
		}

		FILE *fp;
		fp = fopen(args[1], "r+");

		if (fp == NULL)
		{
			fprintf(stderr, "Error: file doesn't exists!\n");
			return (0);
		}
		more(fp, v, h);
		//start_prompt();
		fclose(fp);
	}
	return (0);
};

int		do_builtin(char **args)
{
	if(strcmp(args[0], "cd") == 0)
		sshell_cd(args);
	if(strcmp(args[0], "more") == 0)
		sshell_more(args);
	return (0);
};
