/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:07:41 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/03 19:24:04 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_double_ptr(void **ptr_array)
{
	int	i;

	if (!ptr_array)
		return ;
	i = 0;
	while (ptr_array[i])
	{
		free(ptr_array[i]);
		i++;
	}
	free(ptr_array);
}

void	cleanup_shell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->line)
		free(shell->line);
	if (shell->commands)
		free_commands(shell->commands);
	free(shell);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
