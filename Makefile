# **************************************************************************** #
#                                   Makefile                                   #
# **************************************************************************** #

NAME            := minishell

CC              := cc
CFLAGS          := -Wall -Wextra -Werror -MMD -MP -Iinc -Ilib/libft
LDFLAGS         :=
RLFLAGS         := -lreadline -lncurses

# Try to autodetect readline via pkg-config
PKG_CONFIG      := $(shell command -v pkg-config 2>/dev/null)
ifeq ($(PKG_CONFIG),)
# no pkg-config, keep defaults
else
READLINE_CFLAGS := $(shell pkg-config --cflags readline 2>/dev/null)
READLINE_LIBS   := $(shell pkg-config --libs readline 2>/dev/null)
CFLAGS          += $(READLINE_CFLAGS)
ifneq ($(strip $(READLINE_LIBS)),)
RLFLAGS         := $(READLINE_LIBS)
endif
endif

UNAME_S         := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	# Homebrew default include/lib paths for readline on macOS
	CFLAGS      += -I/opt/homebrew/opt/readline/include
	LDFLAGS     += -L/opt/homebrew/opt/readline/lib
endif

# ---------------------------------------------------------------------------- #
# Libraries                                                                     #
# ---------------------------------------------------------------------------- #

LIBFT_DIR	:= lib/libft
LIBFT_A		:= $(LIBFT_DIR)/libft.a

# ---------------------------------------------------------------------------- #
# Source files                                                                  #
# ---------------------------------------------------------------------------- #

SRC_DIR         := src
OBJ_DIR         := objs
BUILTINS_DIR    := $(SRC_DIR)/builtins
PARSER_DIR      := $(SRC_DIR)/parser
EXEC_DIR        := $(SRC_DIR)/exec
ENV_DIR         := $(SRC_DIR)/env
UTILS_DIR       := $(SRC_DIR)/utils

SRCS            := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/shell_loop.c \
	$(SRC_DIR)/signals.c \
	$(ENV_DIR)/env.c \
	$(ENV_DIR)/env_ops.c \
	$(ENV_DIR)/env_convert.c \
	$(BUILTINS_DIR)/builtins.c \
	$(BUILTINS_DIR)/bi_echo.c \
	$(BUILTINS_DIR)/bi_cd.c \
	$(BUILTINS_DIR)/bi_pwd.c \
	$(BUILTINS_DIR)/bi_export.c \
	$(BUILTINS_DIR)/bi_unset.c \
	$(BUILTINS_DIR)/bi_env.c \
	$(BUILTINS_DIR)/bi_exit.c \
	$(PARSER_DIR)/lexer.c \
	$(PARSER_DIR)/lexer_word.c \
	$(PARSER_DIR)/parser.c \
	$(PARSER_DIR)/parser_argv.c \
	$(PARSER_DIR)/parser_redirs.c \
	$(PARSER_DIR)/eval_line.c \
	$(PARSER_DIR)/eval_utils.c \
	$(PARSER_DIR)/expand.c \
	$(PARSER_DIR)/heredoc.c \
	$(SRC_DIR)/script_prep.c \
	$(SRC_DIR)/script_run.c \
	$(EXEC_DIR)/exec.c \
	$(EXEC_DIR)/exec_utils.c \
	$(EXEC_DIR)/pipeline.c \
	$(EXEC_DIR)/redirs.c \
	$(UTILS_DIR)/strings.c \
	$(UTILS_DIR)/errors.c \
	$(UTILS_DIR)/wildcards.c \
	$(UTILS_DIR)/utils.c

OBJS            := $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS            := $(OBJS:.o=.d)

# ---------------------------------------------------------------------------- #
# Rules                                                                         #
# ---------------------------------------------------------------------------- #

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBFT_A) $(RLFLAGS)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR) --no-print-directory

bonus: CFLAGS += -DBONUS=1
bonus: re

-include $(DEPS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory clean || true

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) --no-print-directory fclean || true

re: fclean all

.PHONY: all clean fclean re bonus valgrind
