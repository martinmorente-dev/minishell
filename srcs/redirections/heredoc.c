/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:23:31 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/05 20:03:58 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

extern int	g_signal_received;

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

static char	*create_temp_filename(void)
{
	char	*pid_str;
	char	*filename;
	char	*tmp;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	tmp = ft_strjoin(".heredoc_", pid_str);
	free(pid_str);
	if (!tmp)
		return (NULL);
	filename = ft_strdup(tmp);
	free(tmp);
	return (filename);
}

int	read_heredoc_lines(char *delimiter, int temp_fd)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (ft_putstr_fd("minishell: unexpected EOF\n", 2),
				0);
		if (g_signal_received == SIGINT)
		{
			free(line);
			return (-1);
		}
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line);
			break ;
		}
		write_line_to_fd(temp_fd, line);
		free(line);
	}
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	char	*tmp;
	int		wfd;
	int		rfd;
	void	(*old)(int);

	if (!delimiter)
		return (-1);
	g_signal_received = 0;
	tmp = create_temp_filename();
	if (!tmp)
		return (-1);
	wfd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (wfd < 0)
		return (perror("heredoc"),
			free(tmp), -1);
	old = signal(SIGINT, heredoc_sigint_handler);
	if (read_heredoc_lines(delimiter, wfd) < 0)
		return (close(wfd), unlink(tmp),
			free(tmp), signal(SIGINT, old), -1);
	close(wfd);
	rfd = open(tmp, O_RDONLY);
	if (rfd < 0)
		return (perror("heredoc"), unlink(tmp),
			free(tmp), signal(SIGINT, old), -1);
	return (unlink(tmp), free(tmp), signal(SIGINT, old), rfd);
}


