/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:34:57 by alberome          #+#    #+#             */
/*   Updated: 2026/03/08 18:23:32 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_child(t_shell *sh, t_cmd *cmd)
{
	char	*path;
	char	**envp;

	if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0])
		_exit(0);
	if (apply_redirections(cmd, NULL, NULL) < 0)
		_exit(1);
	if (is_builtin(cmd->argv[0]))
		_exit(exec_builtin(sh, cmd->argv));
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
	ft_print_perror(cmd->argv[0]);
	_exit(126);
}

static pid_t	launch_pipe_cmd(t_shell *sh, t_cmd *cmd,
		int prev_fd, int *fds)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_print_perror("fork");
		return (-1);
	}
	if (pid != 0)
		return (pid);
	setup_signal_handlers_child();
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
	}
	sh->in_pipe = true;
	run_child(sh, cmd);
	return (0);
}

static int	setup_pipe(size_t i, size_t total, int *fds)
{
	fds[0] = -1;
	fds[1] = -1;
	if (i + 1 < total && pipe(fds) < 0)
	{
		ft_print_perror("pipe");
		return (-1);
	}
	return (0);
}

static int	wait_for_pipeline(pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			else
				last_status = 1;
		}
	}
	return (last_status);
}

int	exec_pipeline_impl(t_shell *sh, t_pipeline *p)
{
	int		prev_fd;
	pid_t	pid;
	size_t	i;
	int		fds[2];

	prev_fd = -1;
	pid = -1;
	i = 0;
	while (i < p->cmds_count)
	{
		if (setup_pipe(i, p->cmds_count, fds) < 0)
			return (1);
		pid = launch_pipe_cmd(sh, &p->cmds[i], prev_fd, fds);
		if (pid == -1)
			return (1);
		if (prev_fd != -1)
			close(prev_fd);
		if (fds[1] != -1)
			close(fds[1]);
		prev_fd = fds[0];
		++i;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (wait_for_pipeline(pid));
}
