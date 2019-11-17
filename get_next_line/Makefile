# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: charmstr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/11 21:40:12 by charmstr          #+#    #+#              #
#    Updated: 2019/11/17 22:22:22 by charmstr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAIN = $1
BUFFER_SIZE = $2

CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE)
CC = gcc
NAME = get_next_line
.PHONY = clean fclean re all bonus
FILE_H = get_next_line.h get_next_line_tests_utils.h
BONUS_FILE_H = ./get_next_line_bonus.h
OBJ_PATH = ./objects/
INCLUDE_PATH = ./
I_FLAGS = -I $(INCLUDE_PATH)

REMOVE_FG = \033[38;5;196m 
CREAT_FG = \033[38;5;46m
BLACK_FG = \033[38;5;0m
BLACK_BG = \033[48;5;0m
CLEAR_COLOR = \033[m

NAME_BG = \033[48;5;39m 
BONUS_BG = \033[48;5;213m
OBJECTS_BG = \033[48;5;11m
RANLIB_BG = \033[48;5;172m

FILES = get_next_line\
		get_next_line_utils\
		get_next_line_tests_utils

BONUS_FILES = get_next_line_bonus\
		get_next_line_utils_bonus

# you can here decide to put srcs/%.c, same for objects, objs/%.o
SRC = $(patsubst %, $(OBJ_PATH)%.c, $(FILES))
OBJ = $(SRC:.c=.o)

SRC_BONUS = $(addprefix $(OBJ_PATH),$(addsuffix .c, $(BONUS_FILES)))
OBJ_BONUS =$(SRC_BONUS:.c=.o)

all: $(NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@echo "\t\t$(CREAT_FG)created the $(OBJECTS_BG)$(BLACK_FG)$@ $(BLACK_BG)$(CREAT_FG) repository$(CLEAR_COLOR)"

$(NAME): $(OBJ) $(FILE_H)
	@echo "\t\t$(CREAT_FG)compiling $(NAME_BG)$(BLACK_FG)$(NAME) $(BLACK_BG)$(CREAT_FG)...$(CLEAR_COLOR)"
	$(CC) $(CFLAGS) $(IFLAGS) $(MAIN) -c -o main.o
	$(CC) $(CFLAGS) $(OBJ) main.o $(IFLAGS) -o $@

$(OBJ): $(OBJ_PATH)%.o: %.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

bonus: $(OBJ_BONUS) $(OBJ) $(FILES_H) $(BONUS_FILE_H)
	@echo "\t\t$(CREAT_FG)compiling $(NAME_BG)$(BLACK_FG)$(NAME) $(BLACK_BG)$(CREAT_FG) with $(BONUS_BG)$(BLACK_FG) BONUSES $(BLACK_BG)$(CREAT_FG)...$(CLEAR_COLOR)"
	$(CC) $(CFLAGS) $(IFLAGS) $(MAIN) -c -o main.o
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_BONUS) main.o $(IFLAGS) -o $(NAME)

$(OBJ_BONUS): $(OBJ_PATH)%.o : %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	@echo "\t\t$(REMOVE_FG)deleting $(OBJECTS_BG)$(BLACK_FG)$(OBJ_PATH) $(BLACK_BG)$(REMOVE_FG) containing all the .o files...$(CLEAR_COLOR)" 
	@rm -rf $(OBJ_PATH)

fclean: clean
	@echo "\t\t$(REMOVE_FG)deleting $(NAME_BG)$(BLACK_FG)$(NAME) $(BLACK_BG)$(REMOVE_FG)...$(CLEAR_COLOR)" 
	@rm -rf $(NAME)

re: fclean all
