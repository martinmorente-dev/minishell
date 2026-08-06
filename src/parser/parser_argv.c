/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:38:50 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 20:58:23 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**resize_temp_argv(char **temp_argv,
		size_t temp_count, size_t new_cap)
{
	char	**new_temp;
	size_t	j;

	new_temp = (char **)ft_calloc(new_cap + 1, sizeof(char *));
	if (!new_temp)
		return (NULL);
	j = 0;
	while (j < temp_count)
	{
		new_temp[j] = temp_argv[j];
		++j;
	}
	free(temp_argv);
	return (new_temp);
}

static int	add_expanded_to_argv(char ***temp_argv, size_t *temp_count,
		size_t *temp_cap, char **expanded)
{
	size_t	exp_len;
	char	**new_temp;
	size_t	j;

	exp_len = ft_strarray_len(expanded);
	if (*temp_count + exp_len >= *temp_cap)
	{
		*temp_cap = *temp_count + exp_len + 8;
		new_temp = resize_temp_argv(*temp_argv, *temp_count, *temp_cap);
		if (!new_temp)
			return (0);
		*temp_argv = new_temp;
	}
	j = 0;
	while (j < exp_len)
	{
		(*temp_argv)[(*temp_count)++] = ft_strdup(expanded[j]);
		++j;
	}
	return (1);
}

static int	process_token(char **toks, size_t *i, size_t ctx[2],
		char ***temp_argv)
{
	char	**expanded;

	if (ft_streq(toks[*i], "|"))
		return ((*i)++, 1);
	if (is_redir_op(toks[*i]))
		return ((*i += 2), 1);
	expanded = expand_wildcards(toks[*i]);
	if (!expanded)
		return ((*i)++, 1);
	if (!add_expanded_to_argv(temp_argv, &ctx[0], &ctx[1], expanded))
	{
		ft_strarray_free(*temp_argv);
		ft_strarray_free(expanded);
		*temp_argv = NULL;
		return (0);
	}
	ft_strarray_free(expanded);
	return ((*i)++, 1);
}

static char	**create_final_argv(char **temp_argv, size_t temp_count)
{
	char	**argv;
	size_t	i;

	if (temp_count == 0)
		return ((char **)ft_calloc(1, sizeof(char *)));
	argv = (char **)ft_calloc(temp_count + 1, sizeof(char *));
	if (!argv)
	{
		ft_strarray_free(temp_argv);
		return (NULL);
	}
	i = 0;
	while (i < temp_count)
	{
		argv[i] = temp_argv[i];
		++i;
	}
	argv[temp_count] = NULL;
	free(temp_argv);
	return (argv);
}

char	**slice_argv(char **toks, size_t start, size_t end)
{
	char	**temp_argv;
	size_t	ctx[2];
	size_t	i;

	temp_argv = NULL;
	ctx[0] = 0;
	ctx[1] = 0;
	i = start;
	while (i < end)
	{
		if (!process_token(toks, &i, ctx, &temp_argv))
			return (NULL);
	}
	if (!temp_argv && ctx[0] == 0)
		return (create_final_argv(NULL, 0));
	if (!temp_argv)
		return (NULL);
	return (create_final_argv(temp_argv, ctx[0]));
}
