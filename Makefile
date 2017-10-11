# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/17 17:47:06 by kbagot            #+#    #+#              #
#    Updated: 2017/10/10 21:08:00 by kbagot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
FLAGS = -g -I include/ -Wall -Werror -Wextra #-fsanitize=address
I_FILES = main.c exec_utility.c utility.c builtin.c echo.c env.c tools.c \
		  tools_two.c builtin_exec_cd.c line_edit.c term_setting.c \
		  cursor_edit.c copy_cut.c line_edit_add.c move_by_word.c \
		  strquotesplit.c splitforquote.c splitforquote_tools.c cursor_move.c \
		  heredoc.c line_edit_cadd.c line_edit_parsing.c line_edit_init.c \
		  parse_token.c show_prompt.c redirection.c exec_parsing.c \
		  builtin_exit.c exec_cmd.c exec_fork_pipe.c line_edit_tools.c \
		  exec_parsing_stock.c redirection_duplicate.c line_edit_qto_mngmt.c
L_FILES = $(I_FILES:.c=.o)
O_FILES = $(addprefix objects/, $(L_FILES))
C_FILES = $(addprefix src/, $(I_FILES))
LIBFT = -L libft/ -lft -ltermcap -lncurses

.PHONY: all clean fclean re

all: $(NAME)
$(NAME): $(C_FILES) include/sh.h Makefile
	@echo "\033[0;35mCompile $(NAME)"
	@make -C libft/
	@gcc -c $(FLAGS) $(C_FILES)
	@mkdir -p objects && mv $(L_FILES) objects/
	@gcc -o $(NAME) $(FLAGS) $(O_FILES) $(LIBFT)
clean:
	@echo "\033[0;35mDelete ft_ls object files"
	@rm -rf objects/
	@make -C libft/ clean
fclean: clean
	@echo "\033[0;35mDelete $(NAME)"
	@rm -rf $(NAME)
	@make -C libft/ fclean
re: fclean all
