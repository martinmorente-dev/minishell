/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:39:55 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 21:31:33 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** make_heredoc_path: generates a unique temp file path using a static counter.
** Returns a malloc'd string; caller must free.
*/
static char	*make_heredoc_path(void)
{
	static int	counter = 0;
	char		*num;
	char		*path;

	num = ft_itoa(counter++);
	if (!num)
		return (NULL);
	path = ft_strjoin3(HEREDOC_TMP_PREFIX, num, "");
	free(num);
	return (path);
}

/*
** fill_heredoc: reads lines via readline until the delimiter is found (or EOF).
** If quoted_delim is false, $VAR and $? are expanded in each line.
** Writes content to fd (the open temp file).
** Returns 0 on success, -1 if SIGINT/EOF received.
*/
static int	write_heredoc_line(t_shell *sh, int fd,
		char *line, bool quoted_delim)
{
	char	*expanded;

	if (!quoted_delim)
	{
		expanded = expand_vars(sh, line);
		free(line);
		if (!expanded)
			return (-1);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

static int	fill_heredoc(t_shell *sh, int fd,
		const char *delim, bool quoted_delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_print_error3(SHELL_NAME, ": warning: "
				"here-document delimited by end-of-file (wanted `", delim);
			write(STDERR_FILENO, "')\n", 2);
			return (0);
		}
		if (ft_streq(line, delim))
		{
			free(line);
			return (0);
		}
		if (write_heredoc_line(sh, fd, line, quoted_delim) < 0)
			return (-1);
	}
}

/*
** process_heredoc: creates a temp file, fills it with heredoc content, closes
** it, then replaces redir->arg with the temp file path (so it can be opened
** as a regular input file during execution).
** Returns 0 on success, -1 on error.
*/
static int	process_heredoc(t_shell *sh, t_redir *redir)
{
	char	*path;
	int		fd;

	path = make_heredoc_path();
	if (!path)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (ft_print_perror(path), free(path), -1);
	if (fill_heredoc(sh, fd, redir->arg, redir->quoted_delim) < 0)
		return (close(fd), unlink(path), free(path), -1);
	close(fd);
	free(redir->arg);
	redir->arg = path;
	redir->type = REDIR_IN;
	return (0);
}

/*
** run_heredocs: iterates all commands in the pipeline and processes any
** REDIR_HEREDOC entries before execution begins.
** Returns 0 on success, -1 if any heredoc failed.
*/
int	run_heredocs(t_shell *sh, t_pipeline *p)
{
	size_t	i;
	size_t	j;

	if (!p || !p->cmds)
		return (0);
	i = 0;
	while (i < p->cmds_count)
	{
		j = 0;
		while (j < p->cmds[i].redirs_count)
		{
			if (p->cmds[i].redirs[j].type == REDIR_HEREDOC
				&& process_heredoc(sh, &p->cmds[i].redirs[j]) < 0)
				return (-1);
			++j;
		}
		++i;
	}
	return (0);
}
