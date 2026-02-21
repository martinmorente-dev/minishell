/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:02:01 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/17 19:34:49 by mmorente         ###   ########.fr       */
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
#include <sys/stat.h>

typedef enum e_token_type
{
	TOKEN_WORD,			// "ls", "hello", "file.txt"
	TOKEN_PIPE,			//  |
	TOKEN_REDIR_IN,		//  <
	TOKEN_REDIR_OUT,	//  >
	TOKEN_REDIR_APPEND,	//  >>
	TOKEN_HEREDOC,		//  <<
	TOKEN_ENV_VAR,		//  $USER, $?
	TOKEN_SQUOTE,		//  '
	TOKEN_DQUOTE		//  "
}	t_token_type;

/*
** echo "hello" | cat > file.txt
** 
** 1. { type: TOKEN_WORD, value: "echo" }
** 2. { type: TOKEN_DQUOTE, value: "hello" }
** 3. { type: TOKEN_PIPE, value: "|" }
** 4. { type: TOKEN_WORD, value: "cat" }
** 5. { type: TOKEN_REDIR_OUT, value: ">" }
** 6. { type: TOKEN_WORD, value: "file.txt" }
*/
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;	
}	t_token;

typedef enum e_redir_type
{
	REDIR_IN,			// <  
	REDIR_OUT,			// > 
	REDIR_APPEND,		// >> 
	REDIR_HEREDOC		// << heredoc
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
	char			**args;		// ["ls", "-la", NULL]
	t_redir			*redirs;
	int				infile;		// FD STDIN_FILENO
	int				outfile;	// FD  STDOUT_FILENO
	struct s_cmd	*next;	
}	t_cmd;


typedef struct s_env
{
	char	*key;// "USER", "HOME", "PATH"
	char	*value; //"student", "/home/student"
	bool	exported;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	t_env			*env;	
	t_cmd			*commands;	
	char			*line;		//readline
	int				exit_code;	// ($?)
	int				exit_flag;	
}	t_minishell;



/* ==================== (PARTNER 1) ==================== */

t_minishell	*init_minishell(char **envp);//++++
void	cleanup_shell(t_minishell *shell);//+++
void	process_line(t_minishell *shell);//+++

void	print_tokens(t_token *tokens);//++++
int	count_tokens(t_token *tokens);//+++
t_token		*tokenize(char *line);//++++

t_token		*create_token(t_token_type type, char *value);//++++

void		add_token(t_token **tokens, t_token *new_token);//++++

void		free_tokens(t_token *tokens);//+++

char		*expand_variables(char *str, t_env *env, int exit_code);//++++

char		*get_env_value(t_env *env, char *key);//++++

t_cmd		*parse_tokens(t_token *tokens, t_env *env);

t_cmd		*create_cmd(void);

bool		add_arg(t_cmd *cmd, char *arg);

bool		add_redir(t_cmd *cmd, t_redir_type type, char *file);

t_cmd		*append_cmd(t_cmd *cmds, t_cmd *new_cmd);

void		free_commands(t_cmd *cmds);

void	print_all_commands(t_cmd *cmds);//++++
void	print_command(t_cmd *cmd);//++++
void	print_all_env(t_env *env);//+++
bool	set_env_value(t_env **env, char *key, char *value);

char		*process_quotes(char *str);

bool		check_quotes(char *str);//+++

/* ==================== (PARTNER 2) ==================== */

int			execute_commands(t_minishell *shell);//--

int			setup_pipes(t_cmd *cmds);

int			create_pipe(int pipefd[2]);

pid_t		fork_and_exec(t_cmd *cmd, t_env *env);

int			wait_children(void);

int			apply_redirections(t_redir *redirs);//--

int			open_redir_file(t_redir *redir);

int			handle_heredoc(char *delimiter);//--

bool		is_builtin(char *cmd);//---

int			exec_builtin(t_minishell *shell, t_cmd *cmd);//---

int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_pwd(void);
int			builtin_export(char **args, t_env **env);
int			builtin_unset(char **args, t_env *env);
int			builtin_env(t_env *env, char **args);
int			builtin_exit(t_minishell *shell, char **args);


char		*find_command_path(char *cmd, t_env *env);//--
char		**env_to_array(t_env *env);

bool		is_executable(char *path);//--

/* ==================== (PARTNER 1) ==================== */
t_env		*init_env(char **envp);//++++
char		*get_env_value(t_env *env, char *key);//+++
bool		set_env_value(t_env **env, char *key, char *value);//+++
bool		unset_env_value(t_env **env, char *key);//+++
void		free_env(t_env *env);//++++

void		setup_signals(void);//+++++

//  SIGINT (Ctrl+C)
void		sigint_handler(int sig);//+++++

//  SIGQUIT (Ctrl+\)
void		sigquit_handler(int sig);//+++++


void		print_error(char *msg);//++++
void		ft_free_double_ptr(void **ptr_array);



/*
**  "ls -la"
** 
** 
** TOKEN_WORD("ls") → TOKEN_WORD("-la")
**
**
** t_cmd {
**     args = ["ls", "-la", NULL],
**     redirs = NULL,
**     infile = 0 (STDIN),
**     outfile = 1 (STDOUT),
**     next = NULL
** }
*/

/*
** "cat file.txt | grep hello"
**
**
** TOKEN_WORD("cat") → TOKEN_WORD("file.txt") → TOKEN_PIPE → 
** TOKEN_WORD("grep") → TOKEN_WORD("hello")
**
**
** cmd1: { args=["cat", "file.txt", NULL], next=cmd2 }
** cmd2: { args=["grep", "hello", NULL], next=NULL }
**
**
** minishell/
** ├── Makefile
** ├── minishell.h    
** ├── libft/            
** │   ├── libft.h
** │   ├── ft_strlen.c
** │   └── ...
** └── srcs/
**     ├── main.c     
**     │
**     ├── lexer/               # PARTNER 1
**     │   ├── tokenize.c      
**     │   ├── tokens_utils.c 
**     │   └── quotes.c       
**     │
**     ├── parser/              # PARTNER 1
**     │   ├── parse.c         
**     │   ├── commands.c      
**     │   └── redirections.c  
**     │
**     ├── expander/            # PARTNER 1
**     │   ├── expand.c       
**     │   └── expand_utils.c 
**     │
**     ├── executor/            # PARTNER 2
**     │   ├── execute.c 
**     │   ├── pipes.c     
**     │   └── processes.c    
**     │
**     ├── builtins/            # PARTNER 2
**     │   ├── builtin_echo.c
**     │   ├── builtin_cd.c
**     │   ├── builtin_pwd.c
**     │   ├── builtin_env.c
**     │   ├── builtin_export.c
**     │   ├── builtin_unset.c
**     │   └── builtin_exit.c
**     │
**     ├── redirections/        # PARTNER 2
**     │   ├── redirections.c  
**     │   └── heredoc.c       
**     │
**     ├── env/                 # PARTNER 1
**     │   ├── env_init.c       
**     │
**     ├── signals/             # PARTNER 1
**     │   └── signals.c       
**     │
**     └── utils/               # PARTNER 1 2
**         ├── errors.c         
**         ├── free.c          
**         └── utils.c         
*/

#endif
