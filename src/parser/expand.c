/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:40:02 by alberome          #+#    #+#             */
/*   Updated: 2026/03/26 18:20:36 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** append_str: appends 'add_len' bytes of 'add' to '*base'.
** Manages the heap buffer; returns new pointer or NULL on alloc failure.
*/
static char	*append_str(char *base, size_t *base_len,
		const char *add, size_t add_len)
{
	char	*new_base;

	if (!add || add_len == 0)
		return (base);
	new_base = (char *)malloc(*base_len + add_len + 1);
	if (!new_base)
	{
		free(base);
		return (NULL);
	}
	ft_memcpy(new_base, base, *base_len);
	ft_memcpy(new_base + *base_len, add, add_len);
	new_base[*base_len + add_len] = '\0';
	free(base);
	*base_len += add_len;
	return (new_base);
}

static char	*expand_one_var(t_shell *sh, const char **s)
{
	const char	*start;
	char		*name;
	char		*val;

	if (**s == '?')
	{
		++(*s);
		return (ft_itoa(sh->last_status));
	}
	if (!(**s == '_' || (**s >= 'A' && **s <= 'Z')
			|| (**s >= 'a' && **s <= 'z')))
		return (NULL);
	start = *s;
	while (**s == '_' || (**s >= 'A' && **s <= 'Z')
		|| (**s >= 'a' && **s <= 'z') || (**s >= '0' && **s <= '9'))
		++(*s);
	name = ft_strndup(start, (size_t)(*s - start));
	if (!name)
		return (ft_strdup(""));
	val = env_get(sh->env, name);
	free(name);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static char	*expand_step(t_shell *sh, char *result,
		size_t *result_len, const char **s)
{
	char		*var_val;
	const char	*seg_start;

	if (**s == '$')
	{
		++(*s);
		var_val = expand_one_var(sh, s);
		if (var_val)
			result = append_str(result, result_len,
					var_val, ft_strlen(var_val));
		else
			result = append_str(result, result_len, "$", 1);
		free(var_val);
		return (result);
	}
	seg_start = *s;
	while (**s && **s != '$')
		++(*s);
	return (append_str(result, result_len,
			seg_start, (size_t)(*s - seg_start)));
}

/*
** expand_vars: expands $VAR and $? in an unquoted or double-quoted string.
** Single-quoted strings should NOT be passed here.
*/
char	*expand_vars(t_shell *sh, const char *input)
{
	char		*result;
	size_t		result_len;
	const char	*s;

	if (!input)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	result_len = 0;
	s = input;
	while (*s)
	{
		result = expand_step(sh, result, &result_len, &s);
		if (!result)
			return (NULL);
	}
	return (result);
}

/*
** expand_vars_dquote: alias for expand_vars; used to clarify call sites
** inside double-quoted strings where expansion is active.
*/
char	*expand_vars_dquote(t_shell *sh, const char *input)
{
	return (expand_vars(sh, input));
}
