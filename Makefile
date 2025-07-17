# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/13 11:10:21 by ysumeral          #+#    #+#              #
#    Updated: 2025/07/18 00:45:16 by ysumeral         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = external/libft/libft.a
SRC =	src/main.c \
		src/memory_free.c \
		src/shell/shell_loop.c \
		src/shell/shell_state.c \
		src/env/env_init.c \
		src/env/env_util.c \
		src/parse/parse_redirect.c \
		src/parse/parse_dollar.c \
		src/parse/parse_pipe.c \
		src/builtin/dispatch.c \
		src/builtin/unset.c \
		src/builtin/cd.c \
		src/builtin/pwd.c \
		src/builtin/echo.c
OBJ = $(SRC:.c=.o)
NAME = minishell

RESET	= \033[0m
GREEN	= \033[0;32m(SUCCESS)$(RESET) -
BLUE	= \033[0;34m(BUILD)$(RESET) -

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE) Linking (Seriously The Best MiniShell Ever)...$(RESET)"
	@make -C ./external/libft -s
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@echo "$(GREEN) MiniShell build successful!$(RESET)"
	@echo "$(GREEN) Authors: ysumeral & makpolat$(RESET)"

clean:
	@make clean -C ./external/libft
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C ./external/libft
	@rm -rf $(NAME)

test: all clean #TODO DELETE THIS

re: fclean all

.PHONY: all clean fclean re