#include "sshell.h"

int		demon = 0;

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

