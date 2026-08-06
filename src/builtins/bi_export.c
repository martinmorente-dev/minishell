/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 21:08:42 by alberome          #+#    #+#             */
/*   Updated: 2026/03/07 21:08:43 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_key_char(char c, bool first)
{
	if (first && (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
		return (true);
	if (!first && (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
			|| (c >= '0' && c <= '9')))
		return (true);
	return (false);
}

static bool	alloc_key_val(const char *s, size_t i, char **key, char **val)
{
	*key = ft_strndup(s, i);
	if (!*key)
		return (false);
	if (s[i] == '=')
	{
		*val = ft_strdup(s + i + 1);
		if (!*val)
		{
			free(*key);
			return (false);
		}
	}
	else
		*val = NULL;
	return (true);
}

static bool	parse_key_value(const char *s, char **out_key, char **out_val)
{
	size_t	i;

	if (!s || !is_valid_key_char(s[0], true))
		return (false);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!is_valid_key_char(s[i], false))
			return (false);
		++i;
	}
	return (alloc_key_val(s, i, out_key, out_val));
}

static void	print_export_env(t_env *env)
{
	t_env	*e;

	e = env;
	while (e)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, e->key, ft_strlen(e->key));
		if (e->value)
		{
			write(STDOUT_FILENO, "=\"", 2);
			write(STDOUT_FILENO, e->value, ft_strlen(e->value));
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		e = e->next;
	}
}

int	bi_export(t_shell *sh, char **argv)
{
	int		ret;
	int		i;
	char	*key;
	char	*val;

	if (!argv[1])
	{
		print_export_env(sh->env);
		return (0);
	}
	ret = 0;
	i = 0;
	while (argv[++i])
	{
		if (!parse_key_value(argv[i], &key, &val))
		{
			ft_print_error3("export: `", argv[i], "': not a valid identifier");
			ret = 1;
			continue ;
		}
		env_set(&sh->env, key, val, true);
		free(key);
		free(val);
	}
	return (ret);
}
