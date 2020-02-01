NAME=sshell

INCLUDES=./include

all: $(NAME)

$(NAME): $(SRCS) include/sshell.h
	@gcc -I$(INCLUDES)  src/*.c -o $(NAME)

clean:
	@/bin/rm -f *.o

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
