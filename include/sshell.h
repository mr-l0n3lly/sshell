#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SSHELL_TOK_BUFSIZE 64
#define SSHELL_TOK_DELIM " \t\r\n\a"

#define CFG_FILE "shconfig"

extern int demon;

typedef	struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env				*envi;

void				save_cfg(char *line);
int					execute_cmd(char *line);
void				start_prompt();
int					sh_init();
void				exit_sh();
int					is_builtin(char **args);
int					sshell_cd(char **args);
int					getmoreenvi(char *name);
void				more(FILE *fp, int v, int h);
int					sshell_more(char **args);
int					do_builtin(char **args);
char				**split_cmd(char *line);
void				do_demon(char **args);

#endif
