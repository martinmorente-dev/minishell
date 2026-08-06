/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:03 by alberome          #+#    #+#             */
/*   Updated: 2026/03/30 18:38:37 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	normalize_chdir_errno(void)
{
	if (errno == ESTALE)
		errno = ENOENT;
}

static char	*get_new_pwd(t_shell *sh, const char *path)
{
	char	*cwd;
	char	*old_pwd;

	cwd = ft_getcwd_dup();
	if (cwd)
		return (cwd);
	old_pwd = env_get(sh->env, "PWD");
	if (!old_pwd || !path)
		return (NULL);
	return (ft_strjoin3(old_pwd, "/", path));
}

static const char	*resolve_cd_path(t_shell *sh, char **argv)
{
	char	*target;

	if (argv[1] && ft_streq(argv[1], "--"))
		argv[1] = NULL;
	if (!argv[1] || ft_streq(argv[1], "~"))
	{
		target = env_get(sh->env, "HOME");
		if (!target)
			ft_print_error3("cd: ", "HOME not set", NULL);
		return (target);
	}
	if (ft_streq(argv[1], "-"))
	{
		target = env_get(sh->env, "OLDPWD");
		if (!target)
			ft_print_error3("cd: ", "OLDPWD not set", NULL);
		else
		{
			write(STDOUT_FILENO, target, ft_strlen(target));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (target);
	}
	return (argv[1]);
}

static int	cd_chdir_error(const char *path, char *oldpwd)
{
	char	*errmsg;

	normalize_chdir_errno();
	errmsg = ft_strjoin3(path, ": ", strerror(errno));
	if (errmsg)
		ft_print_error3("cd: ", errmsg, NULL);
	else
		ft_print_error3("cd: ", path, NULL);
	free(errmsg);
	free(oldpwd);
	return (1);
}

int	bi_cd(t_shell *sh, char **argv)
{
	const char	*path;
	char		*oldpwd;
	char		*newpwd;

	if (argv[1] && !ft_streq(argv[1], "--") && argv[2])
		return (ft_print_error3("cd: ", "too many arguments", NULL), 1);
	if (argv[1] && ft_streq(argv[1], "--") && argv[2] && argv[3])
		return (ft_print_error3("cd: ", "too many arguments", NULL), 1);
	path = resolve_cd_path(sh, argv);
	if (!path)
		return (1);
	oldpwd = ft_getcwd_dup();
	if (chdir(path) != 0)
		return (cd_chdir_error(path, oldpwd));
	newpwd = get_new_pwd(sh, path);
	if (oldpwd)
		env_set(&sh->env, "OLDPWD", oldpwd, true);
	if (newpwd)
		env_set(&sh->env, "PWD", newpwd, true);
	free(oldpwd);
	free(newpwd);
	return (0);
}
