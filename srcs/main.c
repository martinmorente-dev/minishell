/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:01:45 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/08 15:49:41 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();		
	//rl_replace_line("", 0);	
	rl_redisplay();			
}

/*
** sigquit_handler SIGQUIT (Ctrl+\)
**
*/
void	sigquit_handler(int sig)
{
	(void)sig;
}

/*
** setup_signals 
** - SIGINT (Ctrl+C)
** - SIGQUIT (Ctrl+\)
*/
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*shell;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (NULL);
	
	shell->env = init_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	
	shell->commands = NULL;
	shell->line = NULL;
	shell->exit_code = 0;
	shell->exit_flag = 0;
	
	return (shell);
}

void	cleanup_shell(t_minishell *shell)
{
	if (!shell)
		return;
	
	if (shell->env)
		free_env(shell->env);
	
	if (shell->line)
		free(shell->line);
	
	if (shell->commands)
		free_commands(shell->commands);
	
	free(shell);
}

void	process_line(t_minishell *shell)
{
	t_token	*tokens;

	if (!shell->line || shell->line[0] == '\0')
		return;
	
	add_history(shell->line);
	
	tokens = tokenize(shell->line);
	if (!tokens)
	{
		perror("minishell: syntax error");
		shell->exit_code = 2;
		return;
	}
	
	shell->commands = parse_tokens(tokens, shell->env);
	free_tokens(tokens);
	
	if (!shell->commands)
	{
		perror("minishell: parse error");
		shell->exit_code = 2;
		return;
	}
	
	shell->exit_code = execute_commands(shell->commands, shell->env);
	
	free_commands(shell->commands);
	shell->commands = NULL;
}

void	repl_loop(t_minishell *shell)
{
	char	*prompt;

	prompt = "minishell> ";
	
	while (!shell->exit_flag)
	{
		g_signal_received = 0;
		
		shell->line = readline(prompt);
		
		if (!shell->line)
		{
			write(1, "exit\n", 5);
			break;
		}
		
		process_line(shell);
		
		free(shell->line);
		shell->line = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;
	int			exit_code;

	if (argc != 1)
	{
		write(2, "minishell: no arguments expected\n", 34);
		return (1);
	}
	
	(void)argv;	
	
	shell = init_minishell(envp);
	if (!shell)
	{
		write(2, "minishell: initialization failed\n", 34);
		return (1);
	}
	
	setup_signals();
	
	repl_loop(shell);
	
	exit_code = shell->exit_code;
	
	cleanup_shell(shell);
	
	//rl_clear_history();
	clear_history();
	return (exit_code);
}
///rl_clear_history();
//	clear_history();
//rl_replace_line("", 0);
