/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:35:15 by alberome          #+#    #+#             */
/*   Updated: 2026/03/08 18:23:32 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	try_enoexec(char *path, char **envp)
{
	char	*sh_argv[3];

	if (errno != ENOEXEC)
		return ;
	sh_argv[0] = (char *)"sh";
	sh_argv[1] = path;
	sh_argv[2] = NULL;
	execve("/bin/sh", sh_argv, envp);
}

static void	exec_child(t_shell *sh, t_cmd *cmd)
{
	char	*path;
	char	**envp;

	setup_signal_handlers_child();
	if (apply_redirections(cmd, NULL, NULL) < 0)
		_exit(1);
	path = find_executable_in_path(sh->env, cmd->argv[0]);
	if (!path)
		exec_cmd_not_found(cmd->argv[0]);
	if (cmd_is_dir(path))
	{
		ft_print_error3(path, ": ", "Is a directory");
		free(path);
		_exit(126);
	}
	envp = env_to_envp_array(sh->env);
	execve(path, cmd->argv, envp);
	try_enoexec(path, envp);
	ft_print_perror(cmd->argv[0]);
	_exit(126);
}

static int	exec_external(t_shell *sh, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_print_perror("fork");
		return (1);
	}
	if (pid == 0)
		exec_child(sh, cmd);
	status = 0;
	if (waitpid(pid, &status, 0) < 0)
	{
		ft_print_perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	exec_single_builtin(t_shell *sh, t_cmd *cmd)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = -1;
	saved_out = -1;
	if (apply_redirections(cmd, &saved_in, &saved_out) < 0)
	{
		restore_redirections(saved_in, saved_out);
		return (1);
	}
	status = exec_builtin(sh, cmd->argv);
	restore_redirections(saved_in, saved_out);
	return (status);
}

int	exec_pipeline(t_shell *sh, t_pipeline *p)
{
	t_cmd	*cmd;

	if (!p || p->cmds_count == 0)
		return (0);
	if (p->cmds_count > 1)
		return (exec_pipeline_impl(sh, p));
	cmd = &p->cmds[0];
	if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
		return (0);
	if (is_builtin(cmd->argv[0]))
		return (exec_single_builtin(sh, cmd));
	return (exec_external(sh, cmd));
}
