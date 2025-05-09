# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysumeral <ysumeral@student.42istanbul.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 16:55:05 by ysumeral          #+#    #+#              #
#    Updated: 2025/05/08 17:18:18 by ysumeral         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = ./src
LIB_DIR = ./external/libft
INC_DIR = ./include
SRC = $(SRC_DIR)/main.c \
	$(LIB_DIR)/ft_strcmp.c
OBJ = $(SRC:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lreadline

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

test: all clean # bu komut proje bitiminde kaldırılmalı, .o dosyalarını temizler.

re: fclean all

.PHONY: all clean fclean re