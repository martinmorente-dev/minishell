/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:35:52 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 18:06:58 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*next_command_segment(char *p, char **seg)
{
	char	*end;

	while (*p == ' ' || *p == '\t')
		++p;
	*seg = p;
	end = p;
	while (*end && *end != ';')
		++end;
	if (*end == ';')
	{
		*end = '\0';
		return (end + 1);
	}
	return (end);
}

int	run_commands_string(t_shell *sh, const char *cmds)
{
	char	*copy;
	char	*p;
	char	*seg;
	int		status;

	copy = ft_strdup(cmds);
	if (!copy)
		return (1);
	status = 0;
	p = copy;
	while (*p)
	{
		p = next_command_segment(p, &seg);
		if (*seg)
		{
			setup_signal_handlers_noninteractive();
			if (!eval_line(sh, seg, &status))
				status = 1;
			sh->last_status = status;
		}
	}
	free(copy);
	return (status);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		status;

	shell.env = env_create_from_environ(envp);
	shell.last_status = 0;
	shell.running = true;
	shell.in_pipe = false;
	setup_signal_handlers_interactive();
	if (ac > 1)
	{
		if (ft_streq(av[1], "-c") && ac > 2)
		{
			status = run_commands_string(&shell, av[2]);
			env_destroy(&shell.env);
			return (status);
		}
		status = run_script_file(&shell, av[1]);
		env_destroy(&shell.env);
		return (status);
	}
	status = shell_loop(&shell);
	env_destroy(&shell.env);
	return (status);
}
