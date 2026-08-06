/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:37:32 by alberome          #+#    #+#             */
/*   Updated: 2026/03/30 18:37:40 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	match_step(const char **s, const char **p, const char **star,
		const char **match)
{
	if (**p == '*')
	{
		*star = *p;
		(*p)++;
		*match = *s;
	}
	else if (**p == **s || **p == '?')
	{
		(*p)++;
		(*s)++;
	}
	else if (*star)
	{
		*p = *star + 1;
		(*match)++;
		*s = *match;
	}
	else
		return (false);
	return (true);
}

static bool	match_pattern(const char *str, const char *pattern)
{
	const char	*s;
	const char	*p;
	const char	*star;
	const char	*match;

	s = str;
	p = pattern;
	star = NULL;
	match = NULL;
	while (*s)
	{
		if (!match_step(&s, &p, &star, &match))
			return (false);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}

static char	**create_single_result(const char *pattern)
{
	char	**result;

	result = (char **)ft_calloc(2, sizeof(char *));
	if (!result)
		return (NULL);
	result[0] = ft_strdup(pattern);
	result[1] = NULL;
	return (result);
}

static bool	add_match(char ***matches, size_t meta[2], const char *name)
{
	char	**new_matches;

	if (meta[0] >= meta[1])
	{
		if (meta[1] == 0)
			meta[1] = 8;
		else
			meta[1] = meta[1] * 2;
		new_matches = (char **)ft_calloc(meta[1] + 1, sizeof(char *));
		if (!new_matches)
			return (ft_strarray_free(*matches), *matches = NULL, false);
		if (meta[0] > 0)
		{
			ft_memcpy(new_matches, *matches, sizeof(char *) * meta[0]);
			free(*matches);
		}
		*matches = new_matches;
	}
	(*matches)[meta[0]] = ft_strdup(name);
	if (!(*matches)[meta[0]])
		return (ft_strarray_free(*matches), *matches = NULL, false);
	meta[0]++;
	(*matches)[meta[0]] = NULL;
	return (true);
}

char	**expand_wildcards(const char *pattern)
{
	DIR				*dir;
	char			**matches;
	size_t			meta[2];
	struct dirent	*entry;

	if (!pattern || !ft_strchr(pattern, '*'))
		return (create_single_result(pattern));
	dir = opendir(".");
	if (!dir)
		return (create_single_result(pattern));
	meta[0] = 0;
	meta[1] = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (!(entry->d_name[0] == '.' && pattern[0] != '.')
			&& match_pattern(entry->d_name, pattern)
			&& !add_match(&matches, meta, entry->d_name))
			return (closedir(dir), NULL);
		entry = readdir(dir);
	}
	closedir(dir);
	if (meta[0] == 0)
		return (create_single_result(pattern));
	return (matches);
}
