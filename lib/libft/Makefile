# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 17:26:46 by alberome          #+#    #+#              #
#    Updated: 2025/05/23 14:31:04 by alberome         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a

MY_SOURCES= ft_atoi.c \
				ft_bzero.c ft_calloc.c ft_isalnum.c get_next_line.c\
				ft_isalpha.c ft_isascii.c ft_isdigit.c \
				ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
				ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c \
				ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c \
				ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c \
				ft_substr.c ft_strjoin.c ft_strtrim.c \
				ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
				ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
				ft_printf.c ft_tools.c ft_tools2.c
				

SRCS_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
				ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
				ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

MY_OBJECTS=$(MY_SOURCES:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

CFLAGS=-Wall -Werror -Wextra
CC=cc
AR=ar

all: $(NAME)

$(NAME): $(MY_OBJECTS)
		$(AR) rcs  $(NAME) $(MY_OBJECTS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(MY_OBJECTS) $(OBJS_BONUS)
		$(AR) rcs  $(NAME) $(MY_OBJECTS) $(OBJS_BONUS)

clean:
		rm -rf $(MY_OBJECTS) $(OBJS_BONUS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus