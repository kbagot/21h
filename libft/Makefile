# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbagot <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/13 12:31:52 by kbagot            #+#    #+#              #
#    Updated: 2017/04/11 11:46:19 by kbagot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
I_FILES = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memalloc.c ft_memmove.c ft_memset.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strsplit.c ft_strstr.c ft_strsub.c ft_strtrim.c ft_tolower.c ft_toupper.c ft_lstnew.c ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c ft_sqrt.c ft_recursive_factorial.c ft_swap.c ft_is_prime.c ft_isspace.c get_next_line.c add_prt.c printf.c add_length.c init_sub_specif.c make_char_specif.c make_specifier.c precision_mod.c tools_convert.c tools_convert_uni.c width_mod.c tools_uni.c ft_tabdel.c strmsplit.c
C_FILES = $(addprefix srcs/, $(I_FILES))
O_FILES = $(I_FILES:.c=.o)
FLAGS = -Wall -Werror -Wextra
HEADER_FILES = includes/

.PHONY: all clean fclean re

all: $(NAME)
$(NAME): $(C_FILES)
	@echo "\033[0;36mBuild $(NAME)"
	@gcc -c $(FLAGS) $(C_FILES) -I $(HEADER_FILES)
	@ar rc $(NAME) $(O_FILES)
	@ranlib $(NAME)
clean:
	@echo "\033[0;36mDelete libft object files":
	@rm -rf $(O_FILES)
fclean: clean
	@echo "\033[0;36mDelete $(NAME)"
	@rm -rf $(NAME)
re: fclean all
