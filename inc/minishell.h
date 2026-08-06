/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:26:18 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 19:52:14 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"

# define SHELL_NAME "turtleshell"
# define HEREDOC_TMP_PREFIX "/tmp/turtlesh_heredoc_"

/* -------------------------------------------------------------------------- */
/* Data structures                                                            */
/* -------------------------------------------------------------------------- */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

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
	char			*arg;
	bool			quoted_delim;
}	t_redir;

typedef struct s_cmd
{
	char		**argv;
	t_redir		*redirs;
	size_t		redirs_count;
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd	*cmds;
	size_t	cmds_count;
}	t_pipeline;

typedef struct s_shell
{
	t_env	*env;
	int		last_status;
	bool	running;
	bool	in_pipe;
}	t_shell;

/* -------------------------------------------------------------------------- */
/* env                                                                        */
/* -------------------------------------------------------------------------- */

t_env			*env_new(const char *key, const char *value);
t_env			*env_create_from_environ(char **environ);
void			env_destroy(t_env **head);
char			*env_get(t_env *head, const char *key);
bool			env_set(t_env **head, const char *key, const char *value,
					bool ovwr);
bool			env_unset(t_env **head, const char *key);
char			**env_to_envp_array(t_env *head);
size_t			env_count(t_env *head);

/* -------------------------------------------------------------------------- */
/* builtins                                                                   */
/* -------------------------------------------------------------------------- */

bool			is_builtin(const char *name);
int				exec_builtin(t_shell *sh, char **argv);

int				bi_echo(char **argv);
int				bi_cd(t_shell *sh, char **argv);
int				bi_pwd(void);
int				bi_export(t_shell *sh, char **argv);
int				bi_unset(t_shell *sh, char **argv);
int				bi_env(t_shell *sh);
int				bi_exit(t_shell *sh, char **argv);

/* -------------------------------------------------------------------------- */
/* parser                                                                     */
/* -------------------------------------------------------------------------- */

bool			parse_line(t_shell *sh, const char *line, t_pipeline *out);
void			free_pipeline(t_pipeline *p);
char			*expand_vars(t_shell *sh, const char *input);
char			*expand_vars_dquote(t_shell *sh, const char *input);
int				run_heredocs(t_shell *sh, t_pipeline *p);
bool			eval_line(t_shell *sh, const char *line, int *out_status);
char			**simple_lex(t_shell *sh, const char *line);
char			*read_word(t_shell *sh, const char **s);
char			*append_chunk(char *out, size_t *out_len,
					const char *chunk, size_t chunk_len);
bool			is_redir_op(const char *tok);
bool			build_redirs(char **toks, size_t start, size_t end, t_cmd *cmd);
char			**slice_argv(char **toks, size_t start, size_t end);
char			*trim_segment(char *segment);
int				eval_subshell(t_shell *sh, char *inner);

t_redir_type	redir_type_for(const char *op);
/* -------------------------------------------------------------------------- */
/* exec                                                                       */
/* -------------------------------------------------------------------------- */

int				exec_pipeline(t_shell *sh, t_pipeline *p);
int				exec_pipeline_impl(t_shell *sh, t_pipeline *p);
int				apply_redirections(t_cmd *cmd, int *saved_in, int *saved_out);
void			restore_redirections(int saved_in, int saved_out);
bool			cmd_is_dir(const char *path);
char			*find_executable_in_path(t_env *env, const char *file);
void			exec_cmd_not_found(const char *name);

/* -------------------------------------------------------------------------- */
/* signals                                                                    */
/* -------------------------------------------------------------------------- */

void			setup_signal_handlers_interactive(void);
void			setup_signal_handlers_noninteractive(void);
void			setup_signal_handlers_child(void);

/* -------------------------------------------------------------------------- */
/* utils                                                                      */
/* -------------------------------------------------------------------------- */

char			*ft_strjoin3(const char *a, const char *b, const char *c);
char			**ft_strarray_dup(char **arr);
void			ft_strarray_free(char **arr);
size_t			ft_strarray_len(char **arr);
void			ft_print_perror(const char *prefix);
void			ft_print_error3(const char *a, const char *b, const char *c);
char			*ft_getcwd_dup(void);
void			ft_malloc_error(void);
char			**expand_wildcards(const char *pattern);

int				shell_loop(t_shell *sh);

/* script helpers */
int				reset_fd_to_start(const char *path, int *fd);
int				is_elf_fd(int fd);
int				open_script_fd(const char *path, int *fd_out);
int				report_elf_and_close_if_needed(const char *path, int fd);
int				report_reset_error(const char *path, int *fd);
void			process_script_lines(int fd, t_shell *sh, int *last_status);
int				prepare_script_fd(const char *path, int *out_fd);
int				run_script_file(t_shell *sh, const char *path);

/* noninteractive mode */
int				run_commands_string(t_shell *sh, const char *cmds);

#endif
