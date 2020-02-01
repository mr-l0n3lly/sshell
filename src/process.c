#include "sshell.h"


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

