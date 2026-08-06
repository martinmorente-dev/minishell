/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:35:29 by alberome          #+#    #+#             */
/*   Updated: 2026/03/08 18:30:54 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && (st.st_mode & S_IFMT) != S_IFDIR
		&& access(path, X_OK) == 0)
		return (true);
	return (false);
}

static char	*join_path(const char *dir, const char *file)
{
	if (!dir || !*dir)
		return (ft_strdup(file));
	return (ft_strjoin3(dir, "/", file));
}

static char	*search_in_dirs(const char *paths, const char *file)
{
	char	*p;
	char	*sep;
	char	*dir;
	char	*cand;

	p = (char *)paths;
	while (1)
	{
		sep = p;
		while (*sep && *sep != ':')
			++sep;
		dir = ft_strndup(p, (size_t)(sep - p));
		cand = join_path(dir, file);
		free(dir);
		if (cand && is_executable(cand))
			return (cand);
		free(cand);
		if (!*sep)
			break ;
		p = sep + 1;
	}
	return (NULL);
}

char	*find_executable_in_path(t_env *env, const char *file)
{
	char	*path_var;

	if (!file || !*file)
		return (NULL);
	if (ft_strchr(file, '/'))
	{
		if (is_executable(file))
			return (ft_strdup(file));
		return (NULL);
	}
	path_var = env_get(env, "PATH");
	if (!path_var)
		return (NULL);
	return (search_in_dirs(path_var, file));
}

void	exec_cmd_not_found(const char *name)
{
	struct stat	st;

	if (ft_strchr(name, '/') && cmd_is_dir(name))
	{
		ft_print_error3(name, ": ", "Is a directory");
		_exit(126);
	}
	if (ft_strchr(name, '/') && stat(name, &st) == 0)
	{
		ft_print_error3(name, ": ", "Permission denied");
		_exit(126);
	}
	if (ft_strchr(name, '/'))
	{
		ft_print_error3(name, ": ", "No such file or directory");
		_exit(127);
	}
	ft_print_error3(name, ": ", "command not found");
	_exit(127);
}
