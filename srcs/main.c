/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:01:45 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/03 19:08:56 by mmorente         ###   ########.fr       */
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
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

void	process_line(t_minishell *shell)
{
	t_token	*tokens;

	if (!shell->line || shell->line[0] == '\0')
		return ;
	add_history(shell->line);
	tokens = tokenize(shell->line);
	if (!tokens)
	{
		perror("minishell: syntax error");
		shell->exit_code = 2;
		return ;
	}
	shell->commands = parse_tokens(tokens, shell->env);
	free_tokens(tokens);
	if (!shell->commands)
	{
		perror("minishell: parse error");
		shell->exit_code = 2;
		return ;
	}
	shell->exit_code = execute_commands(shell);
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
			break ;
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
	clear_history();
	return (exit_code);
}
