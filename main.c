#include "sshell.h"

t_env	*envi;
int		demon = 0;

void	save_cfg(char *line)
{
	char *temp = (char*)malloc(sizeof(char) * strlen(line) + 1);
	
	strcpy(temp, line);

	char *token;

	token = strtok(temp, " ");

	if(envi == NULL)
	{
		envi = (t_env*)malloc(sizeof(t_env));
		envi->name = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(envi->name, token);
		token = strtok(NULL, " ");
		envi->value = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(envi->value, token);
		envi->next = NULL; 
	}
	else
	{
		t_env *tmp = envi;

		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = (t_env*)malloc(sizeof(t_env));
		tmp = tmp->next;
		
		tmp->name = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(tmp->name, token);
		token = strtok(NULL, " ");
		tmp->value = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(tmp->value, token);
		tmp->next = NULL; 
	}
	free(temp);
}

int		sh_init()
{
	envi =	NULL;
	FILE	*fd;

	fd = fopen(CFG_FILE, "r");
	if(fd == NULL)
	{
		printf("There isn't a config file in root directory!\n");
		exit(EXIT_FAILURE);
	}

	char	*line = NULL;
    size_t	len = 0;
    ssize_t	read;

    while ((read = getline(&line, &len, fd)) != -1)
	{
        //printf("%s", line);
		line[strlen(line) - 1] = '\0';
		save_cfg(line);
    }

    fclose(fd);
    if (line)
        free(line);
    return (0);
}

void	exit_sh()
{
	free(envi);
}

char	**split_cmd(char *line)
{
	int bufsize = SSHELL_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "sshell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, SSHELL_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += SSHELL_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				fprintf(stderr, "SHELL: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, SSHELL_TOK_DELIM);
	}
  
	tokens[position] = NULL;
	if (strcmp(tokens[position - 1], "&") == 0)
	{
		tokens[position - 1] = NULL;
		demon = 1;
	}
	return tokens;
}

int		is_builtin(char **args)
{
	if(strcmp(args[0], "more") == 0)
		return (1);
	if(strcmp(args[0], "cd") == 0)
		return (1);

	return (0);
}

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
}

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
}

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
}

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
}

int		do_builtin(char **args)
{
	if(strcmp(args[0], "cd") == 0)
		sshell_cd(args);
	if(strcmp(args[0], "more") == 0)
		sshell_more(args);
	return (0);
}

void	do_demon(char **args)
{
	pid_t pid;

    pid = fork();
    
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
	{
		return;
	}
	else
	{
    
		if (setsid() < 0)
			exit(EXIT_FAILURE);
		signal(SIGCHLD, SIG_IGN);
		signal(SIGHUP, SIG_IGN);

		if(execvp(args[0], args) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
}

int		execute_cmd(char *line)
{
	pid_t pid, wpid;
	int status;

	char **args = split_cmd(line);

	if(is_builtin(args) == 1)
	{
		do_builtin(args);
		return (0);
	}
	else if (demon == 1)
	{
		demon = 0;
		do_demon(args);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (execvp(args[0], args) == -1)
			{
				perror("sshell");
			}
		
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("sshell");
		}
		else
		{
			do
			{
				wpid = waitpid(pid, &status, WUNTRACED);
			}
			while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}

	return (1);
}

void	start_prompt()
{
	char		*line = "null";
	size_t		len = 0;
	size_t		read;

	while(1)
	{
		if(strlen(line) > 1)
		{
			printf("\033[0;32m");
			printf("sshell $> ");
			printf("\033[0m");
		}
		
		read = getline(&line, &len, stdin);
		line[strlen(line) - 1] = '\0';
	
		if(strlen(line) > 1)
			if(strcmp(line, "exit") == 0)
			{
				exit_sh();
				break;
			}
			else
				execute_cmd(line);
	}

}

int		main(void)
{
	if(sh_init() != 0)
	{
		printf("There is a error initing the shell!!\n");
		return (-1);
	}

	start_prompt();

	return (0);
}
