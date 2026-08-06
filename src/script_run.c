/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:36:32 by alberome          #+#    #+#             */
/*   Updated: 2026/03/30 18:36:31 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_script_lines(int fd, t_shell *sh, int *last_status)
{
	char	*line;
	size_t	len;

	line = get_next_line(fd);
	while (line != NULL)
	{
		len = ft_strlen(line);
		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
			line[--len] = '\0';
		if (*line)
			eval_line(sh, line, last_status);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
}

int	prepare_script_fd(const char *path, int *out_fd)
{
	int	fd;
	int	rc;

	rc = open_script_fd(path, &fd);
	if (rc != 0)
		return (rc);
	rc = report_elf_and_close_if_needed(path, fd);
	if (rc != 0)
		return (rc);
	rc = report_reset_error(path, &fd);
	if (rc != 0)
		return (rc);
	*out_fd = fd;
	return (0);
}

int	run_script_file(t_shell *sh, const char *path)
{
	int	fd;
	int	last_status;
	int	rc;

	rc = prepare_script_fd(path, &fd);
	if (rc != 0)
		return (rc);
	last_status = 0;
	process_script_lines(fd, sh, &last_status);
	close(fd);
	return (last_status);
}
