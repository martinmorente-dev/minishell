/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:07:41 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/10 19:04:10 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command(t_cmd *cmd)
{
	int		i;
	t_redir	*redir;

	if (!cmd)
		return;
	
	printf("Command:\n");
	
	printf("  Args: ");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("[%s] ", cmd->args[i]);
			i++;
		}
	}
	printf("\n");
	
	redir = cmd->redirs;
	while (redir)
	{
		printf("  Redir: type=%d file=%s\n", redir->type, redir->file);
		redir = redir->next;
	}
	
	printf("  FDs: in=%d out=%d\n", cmd->infile, cmd->outfile);
}


void	print_all_commands(t_cmd *cmds)
{
	int	i;

	i = 1;
	while (cmds)
	{
		printf("\n=== Command %d ===\n", i);
		print_command(cmds);
		cmds = cmds->next;
		i++;
	}
}


void    ft_free_double_ptr(void **ptr_array)
{
    int i;

    if (!ptr_array)
        return;
    i = 0;
    while (ptr_array[i])
    {
        free(ptr_array[i]);
        i++;
    }
    free(ptr_array);
}