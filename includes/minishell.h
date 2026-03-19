/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:02:01 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/19 21:27:54 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"
# include <sys/stat.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;	
}	t_token;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				fd;
	struct s_redir	*next;
}	t_redir;


typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
}	t_cmd;


typedef struct s_env
{
	char			*key;
	char			*value;
	bool			exported;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	t_env			*env;
	t_cmd			*commands;
	char			*line;
	int				exit_code;
	int				exit_flag;
}	t_minishell;


t_minishell	*init_minishell(char **envp);

t_token		*create_token(t_token_type type, char *value);
t_token		*tokenize(char *line);
t_token		*create_op_token(t_token_type type, char *op, int len);
t_token		*get_operator_token(char *line, int *i);

t_env		*init_env(char **envp);
t_env		*create_env_node(char *key, char *value);

t_cmd		*parse_tokens(t_token *tokens, t_env *env);
t_cmd		*create_cmd(void);
t_cmd		*append_cmd(t_cmd *cmds, t_cmd *new_cmd);

pid_t		fork_and_exec(t_cmd *cmd, t_env *env);

void		print_tokens(t_token *tokens);
void		cleanup_shell(t_minishell *shell);
void		process_line(t_minishell *shell);
void		add_token(t_token **tokens, t_token *new_token);
void		free_commands(t_cmd *cmds);
void		free_tokens(t_token *tokens);
void		print_all_commands(t_cmd *cmds);
void		print_command(t_cmd *cmd);
void		print_all_env(t_env *env);
void		free_env(t_env *env);
void		setup_signals(void);
void		sigint_handler(int sig);
void		sigquit_handler(int sig);
void		print_error(char *msg);
void		ft_free_double_ptr(void **ptr_array);
void		cleanup_shell(t_minishell *shell);
void		add_env_to_list(t_env **env_list, t_env *new_node);
void		write_line_to_fd(int fd, char *line);
void		free_commands(t_cmd *cmds);
void		handle_quote(char *line, char c, int *i, t_token **tokens);
void		skip_quote(char *quote, int *i);
void		add_word_token(char *line, int *i, t_token **tokens);
void		handle_operator(char *line, int *i, t_token **tokens);
void		toggle_quote_state(char *quote, char c, int *i);
void		handle_pipe(t_cmd **cur_cmd, t_cmd *cmds);
void		handle_dollar(char *str, int *i, int *j, t_env *env);

int			count_tokens(t_token *tokens);
int			execute_commands(t_minishell *shell);
int			setup_pipes(t_cmd *cmds);
int			create_pipe(int pipefd[2]);
int			skip_whitespace(char *line, int i);
int			detect_redir_type(char c, char next, t_token_type *type, char *op);
int			is_whitespace(char c);


char		*expand_variables(char *str, t_env *env);
char		*get_env_value(t_env *env, char *key);
char		*process_quotes(char *str);
char		*find_command_path(char *cmd, t_env *env);
char		**env_to_array(t_env *env);
char		*get_env_value(t_env *env, char *key);
char		*get_quoted_word(char *line, int *i, char quote);


bool		add_arg(t_cmd *cmd, char *arg);
bool		add_redir(t_cmd *cmd, t_redir_type type, char *file);
bool		set_env_value(t_env **env, char *key, char *value);
bool		check_quotes(char *str);
bool		is_builtin(char *cmd);
bool		is_executable(char *path);
bool		set_env_value(t_env **env, char *key, char *value);
bool		unset_env_value(t_env **env, char *key);
bool		is_operator(char c);
bool		is_quote(char c);


int			handle_redir(t_cmd *cmd, t_token **tok, t_env *env, t_cmd *cmds);
int			wait_children(void);
int			apply_redirections(t_redir *redirs);
int			open_redir_file(t_redir *redir);
int			handle_heredoc(char *delimiter);
int			exec_builtin(t_minishell *shell, t_cmd *cmd);
int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_pwd(void);
int			builtin_export(char **args, t_env **env);
int			builtin_unset(char **args, t_env **env);
int			builtin_env(t_env *env, char **args);
int			builtin_exit(t_minishell *shell, char **args);
int			is_redir(int type);
int			handle_word(t_cmd *cmd, t_token *tok, t_env *env, t_cmd *cmds);


#endif
