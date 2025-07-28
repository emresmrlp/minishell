# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysumeral < ysumeral@student.42istanbul.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/13 11:10:21 by ysumeral          #+#    #+#              #
#    Updated: 2025/07/28 21:21:19 by ysumeral         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT = external/libft/libft.a
SRC =	src/main.c \
		src/memory_free.c \
		src/shell/shell_loop.c \
		src/shell/shell_state.c \
		src/parse/parse_redirect.c \
		src/parse/parse_dollar.c \
		src/parse/parse_pipe.c \
		src/builtin/unset.c \
		src/builtin/cd.c \
		src/builtin/pwd.c \
		src/builtin/echo.c \
		src/builtin/exit.c \
		src/builtin/export.c \
		src/builtin/export_util.c \
		src/builtin/env.c \
		src/builtin/env_util.c \
		src/execute/execute.c \
		src/execute/execute_single.c \
		src/execute/execute_multiple.c \
		src/execute/execute_util.c \
		src/execute/execute_builtin.c \
		src/execute/execute_redirection.c \
		src/parse/find_redirect.c

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

valgrind: $(NAME)
	@echo "$(BLUE) Running Valgrind with readline suppression...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=.valgrind_suppress --track-origins=yes ./$(NAME)

valgrind-clean: $(NAME)
	@echo "$(BLUE) Running Valgrind (clean output)...$(RESET)"
	valgrind --leak-check=full --suppressions=.valgrind_suppress ./$(NAME)

valgrind-debug: $(NAME)
	@echo "$(BLUE) Running Valgrind with full debug...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(NAME)

valgrind-gen-suppress: $(NAME)
	@echo "$(BLUE) Generating suppressions...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --gen-suppressions=yes ./$(NAME)

.PHONY: all clean fclean re valgrind valgrind-clean valgrind-debug valgrind-gen-suppress