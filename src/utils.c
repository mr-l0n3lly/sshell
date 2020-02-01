#include "sshell.h"

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

