# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: makpolat <makpolat@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/13 11:10:21 by ysumeral          #+#    #+#              #
#    Updated: 2025/07/17 16:20:06 by makpolat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = external/libft/libft.a
SRC =	src/parse_pipe.c \
		src/main.c \
		src/env_init.c \
		src/env_manage.c \
		src/input_read.c \
		src/parse_redirect.c \
		src/parse_dollar.c \
		src/builtin.c  \
		src/builtin_utils.c
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