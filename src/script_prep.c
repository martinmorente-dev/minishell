/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_prep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:36:57 by alberome          #+#    #+#             */
/*   Updated: 2026/03/30 18:36:52 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	reset_fd_to_start(const char *path, int *fd)
{
	struct stat	st;

	if (fstat(*fd, &st) < 0)
		return (-1);
	if (S_ISREG(st.st_mode))
	{
		if (close(*fd) < 0)
			return (-1);
		*fd = open(path, O_RDONLY);
		if (*fd < 0)
			return (-1);
		return (0);
	}
	errno = ESPIPE;
	return (-1);
}

int	is_elf_fd(int fd)
{
	unsigned char	magic[4];
	ssize_t			r;

	r = read(fd, magic, sizeof(magic));
	if (r == (ssize_t) sizeof(magic)
		&& magic[0] == 0x7F
		&& magic[1] == 'E'
		&& magic[2] == 'L'
		&& magic[3] == 'F')
		return (1);
	return (0);
}

int	open_script_fd(const char *path, int *fd_out)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_print_perror((char *)path);
		if (errno == ENOENT)
			return (127);
		return (126);
	}
	*fd_out = fd;
	return (0);
}

int	report_elf_and_close_if_needed(const char *path, int fd)
{
	if (is_elf_fd(fd))
	{
		ft_print_error3((char *)path, ": ", "cannot execute binary file");
		close(fd);
		return (126);
	}
	return (0);
}

int	report_reset_error(const char *path, int *fd)
{
	if (reset_fd_to_start(path, fd) < 0)
	{
		if (errno == ESPIPE)
			ft_print_error3((char *)path, ": ",
				"seek not supported on this file type");
		else
			ft_print_perror((char *)path);
		return (126);
	}
	return (0);
}
