/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 12:02:05 by marvin            #+#    #+#             */
/*   Updated: 2026/02/21 12:02:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int builtin_unset(char **args, t_env **env)
{
    int i = 1;
    
    while (args[i])
    {
        unset_env_value(env, args[i])
        i++;
    }
    return (0);
}   