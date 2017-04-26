# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/17 17:47:06 by kbagot            #+#    #+#              #
#    Updated: 2017/04/26 17:42:21 by kbagot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
FLAGS = -g -Wall -Werror -Wextra
I_FILES = main.c exec_utility.c utility.c builtin.c echo.c env.c tools.c \
		  tools_two.c builtin_exec_cd.c line_edit.c term_setting.c \
		  cursor_edit.c copy_cut.c line_edit_add.c move_by_word.c
O_FILES = $(I_FILES:.c=.o)
C_FILES = $(addprefix src/, $(I_FILES))
LIBFT = -L libft/ -lft -ltermcap -lncurses

.PHONY: all clean fclean re

all: $(NAME)
$(NAME): $(C_FILES) include/minishell.h Makefile
	@echo "\033[0;35mCompile $(NAME)"
	@make -C libft/
	@gcc -c $(FLAGS) $(C_FILES)
	@gcc -o $(NAME) $(FLAGS) $(O_FILES) $(LIBFT) 
clean:
	@echo "\033[0;35mDelete ft_ls object files"
	@rm -rf $(O_FILES)
	@make -C libft/ clean
fclean: clean
	@echo "\033[0;35mDelete $(NAME)"
	@rm -rf $(NAME)
	@make -C libft/ fclean
re: fclean all
