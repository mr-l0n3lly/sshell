#include "sshell.h"

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

