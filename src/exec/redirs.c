/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:34:48 by alberome          #+#    #+#             */
/*   Updated: 2026/03/07 21:34:50 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir_fd(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_IN)
		fd = open(redir->arg, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd < 0)
	{
		if (redir->type == REDIR_IN)
			ft_print_error3(SHELL_NAME ": ", redir->arg,
				": No such file or directory");
		else
			ft_print_perror(redir->arg);
		return (-1);
	}
	return (fd);
}

static int	apply_single_redir(t_redir *r, int *saved_in, int *saved_out)
{
	int	fd;

	fd = open_redir_fd(r);
	if (fd < 0)
		return (-1);
	if (r->type == REDIR_IN)
	{
		if (saved_in && *saved_in == -1)
			*saved_in = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		if (saved_out && *saved_out == -1)
			*saved_out = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_cmd *cmd, int *saved_in, int *saved_out)
{
	size_t	i;

	if (saved_in)
		*saved_in = -1;
	if (saved_out)
		*saved_out = -1;
	if (!cmd || cmd->redirs_count == 0)
		return (0);
	i = 0;
	while (i < cmd->redirs_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			++i;
			continue ;
		}
		if (apply_single_redir(&cmd->redirs[i], saved_in, saved_out) < 0)
			return (-1);
		++i;
	}
	return (0);
}

void	restore_redirections(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out >= 0)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}

bool	cmd_is_dir(const char *path)
{
	struct stat	st;

	if (!path)
		return (false);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (true);
	return (false);
}
