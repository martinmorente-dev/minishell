# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/05 17:23:54 by mafarino          #+#    #+#              #
#    Updated: 2026/02/05 17:24:23 by mafarino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I./includes -I./libft
LIBS		= -L./libft -lft -lreadline

SRC_DIR		= srcs
OBJ_DIR		= objs
INC_DIR		= includes
LIBFT_DIR	= libft

LEXER_DIR	= $(SRC_DIR)/lexer
PARSER_DIR	= $(SRC_DIR)/parser
EXPAND_DIR	= $(SRC_DIR)/expander
EXEC_DIR	= $(SRC_DIR)/executor
BUILTIN_DIR	= $(SRC_DIR)/builtins
REDIR_DIR	= $(SRC_DIR)/redirections
ENV_DIR		= $(SRC_DIR)/env
SIGNAL_DIR	= $(SRC_DIR)/signals
UTILS_DIR	= $(SRC_DIR)/utils

MAIN_SRC	= $(SRC_DIR)/main.c

LEXER_SRC	= $(LEXER_DIR)/tokenize.c \
			  $(LEXER_DIR)/tokens_utils.c \
			  $(LEXER_DIR)/quotes.c \
			  $(LEXER_DIR)/token_list.c

PARSER_SRC	= $(PARSER_DIR)/parse.c \
			  $(PARSER_DIR)/parse_commands.c \
			  $(PARSER_DIR)/parse_redirections.c \
			  $(PARSER_DIR)/parse_utils.c \
			  $(PARSER_DIR)/syntax_check.c

EXPAND_SRC	= $(EXPAND_DIR)/expand.c \
			  $(EXPAND_DIR)/expand_utils.c \
			  $(EXPAND_DIR)/expand_vars.c \
			  $(EXPAND_DIR)/quote_removal.c

EXEC_SRC	= $(EXEC_DIR)/execute.c \
			  $(EXEC_DIR)/execute_utils.c \
			  $(EXEC_DIR)/pipes.c \
			  $(EXEC_DIR)/processes.c \
			  $(EXEC_DIR)/path.c

BUILTIN_SRC	= $(BUILTIN_DIR)/builtin_echo.c \
			  $(BUILTIN_DIR)/builtin_cd.c \
			  $(BUILTIN_DIR)/builtin_pwd.c \
			  $(BUILTIN_DIR)/builtin_env.c \
			  $(BUILTIN_DIR)/builtin_export.c \
			  $(BUILTIN_DIR)/builtin_unset.c \
			  $(BUILTIN_DIR)/builtin_exit.c \
			  $(BUILTIN_DIR)/builtin_utils.c

REDIR_SRC	= $(REDIR_DIR)/redirections.c \
			  $(REDIR_DIR)/redir_utils.c \
			  $(REDIR_DIR)/heredoc.c

ENV_SRC		= $(ENV_DIR)/env_init.c 

SIGNAL_SRC	= $(SIGNAL_DIR)/signals.c

UTILS_SRC	= $(UTILS_DIR)/errors.c \
			  $(UTILS_DIR)/free.c \
			  $(UTILS_DIR)/utils.c \
			  $(UTILS_DIR)/string_utils.c

SRCS		= $(MAIN_SRC) \
			  $(LEXER_SRC) \
			  $(PARSER_SRC) \
			  $(EXPAND_SRC) \
			  $(EXEC_SRC) \
			  $(BUILTIN_SRC) \
			  $(REDIR_SRC) \
			  $(ENV_SRC) \
			  $(SIGNAL_SRC) \
			  $(UTILS_SRC)

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS)  -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

norm:
	norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_DIR)

valgrind: $(NAME)
	valgrind --leak-check=full \
			  --show-leak-kinds=all \
			  --track-origins=yes \
			  --suppressions=.ignore_readline_leaks.supp \
			  ./$(NAME)

debug: CFLAGS += -g3 -fsanitize=address
debug: re

.PHONY: all clean fclean re norm valgrind debug
