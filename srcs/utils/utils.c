/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmorente <mmorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 15:46:38 by mafarino          #+#    #+#             */
/*   Updated: 2026/03/03 19:23:13 by mmorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	print_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}
void	print_command(t_cmd *cmd)
{
	int		i;
	t_redir	*redir;

	if (!cmd)
		return ;
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
