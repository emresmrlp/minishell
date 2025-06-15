NAME = minishell

SRC = src/parse_pipe.c \
		src/main.c \
		src/parse_redirect.c \

OBJ = $(SRC:.c=.o) 


LIBFT=Libft/libft.a

CC = gcc -Wall -Wextra -Werror


all: $(NAME)

$(NAME): $(OBJ)
	make -C ./Libft -s
	$(CC) -o $(NAME) $(SRC) $(LIBFT) -lreadline -s

clean:
	rm -rf src/*.o
	make -C ./Libft clean

fclean: clean
	rm -rf $(NAME)
	make -C ./Libft fclean

re: fclean all

.PHONY: all clean fclean re