/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:23:31 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/08 15:34:58 by mafarino         ###   ########.fr       */
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

static void	write_line_to_fd(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static int	read_heredoc_lines(char *delimiter, int temp_fd)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			return (0);
		}
		
		if (g_signal_received == SIGINT)
		{
			free(line);
			return (-1);
		}
		
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line);
			break;
		}
		
		write_line_to_fd(temp_fd, line);
		free(line);
	}
	
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	char				*temp_filename;
	int					temp_fd;
	int					read_fd;
	void				(*old_sigint)(int);

	if (!delimiter)
		return (-1);
	
	g_signal_received = 0;
	
    temp_filename = create_temp_filename();
	if (!temp_filename)
		return (-1);
	
	temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd < 0)
	{
		perror("heredoc");
		free(temp_filename);
		return (-1);
	}
	
	old_sigint = signal(SIGINT, heredoc_sigint_handler);
	
	if (read_heredoc_lines(delimiter, temp_fd) < 0)
	{
		close(temp_fd);
		unlink(temp_filename);
		free(temp_filename);
		signal(SIGINT, old_sigint);
		return (-1);
	}
	
	close(temp_fd);
	
	read_fd = open(temp_filename, O_RDONLY);
	if (read_fd < 0)
	{
		perror("heredoc");
		unlink(temp_filename);
		free(temp_filename);
		signal(SIGINT, old_sigint);
		return (-1);
	}
	
	unlink(temp_filename);
	free(temp_filename);
	
	signal(SIGINT, old_sigint);
	
	return (read_fd);
}
