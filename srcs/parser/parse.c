/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafarino <mafarino@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:03:41 by mafarino          #+#    #+#             */
/*   Updated: 2026/02/08 15:44:22 by mafarino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(char *str)
{
	int		i;

	if (str[0] == '?')
		return (ft_strdup("?"));
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (ft_strdup(""));
	return (ft_substr(str, 0, i));
}

static char	*expand_single_var(char *str, char *dollar, t_env *env, int exit_code)
{
	char	*before;
	char	*var_name;
	char	*var_value;
	char	*after;
	char	*result;
	char	*tmp;

	before = ft_substr(str, 0, dollar - str);
	var_name = get_var_name(dollar + 1);
	if (ft_strncmp(var_name, "?", 2) == 0)
		var_value = ft_itoa(exit_code);
	else
	{
		var_value = get_env_value(env, var_name);
		if (var_value)
			var_value = ft_strdup(var_value);
		else
			var_value = ft_strdup("");
	}
	after = ft_strdup(dollar + ft_strlen(var_name) + 1);
	tmp = ft_strjoin(before, var_value);
	result = ft_strjoin(tmp, after);
	free(before);
	free(var_name);
	free(var_value);
	free(after);
	free(tmp);
	return (result);
}

char	*expand_variables(char *str, t_env *env, int exit_code)
{
	char	*result;
	char	*dollar;
	char	*tmp;

	if (!str)
		return (NULL);
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	while (1)
	{
		dollar = ft_strchr(result, '$');
		if (!dollar)
			break;
		tmp = expand_single_var(result, dollar, env, exit_code);
		free(result);
		if (!tmp)
			return (NULL);
		result = tmp;
	}
	return (result);
}

bool	check_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote == 0);
}

char	*process_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && !quote)
		{
			quote = str[i];
			i++;
		}
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->infile = STDIN_FILENO;
	cmd->outfile = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

bool	add_arg(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		count;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (false);
	count = 0;
	while (cmd->args && cmd->args[count])
	{
		new_args[count] = cmd->args[count];
		count++;
	}
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
	{
		free(new_args);
		return (false);
	}
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (true);
}

bool	add_redir(t_cmd *cmd, t_redir_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (false);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	if (!new_redir->file)
	{
		free(new_redir);
		return (false);
	}
	new_redir->fd = -1;
	new_redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (true);
}

t_cmd	*append_cmd(t_cmd *cmds, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!cmds)
		return (new_cmd);
	current = cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
	return (cmds);
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;
	int		i;

	while (cmds)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		i = 0;
		while (tmp_cmd->args && tmp_cmd->args[i])
			free(tmp_cmd->args[i++]);
		free(tmp_cmd->args);
		while (tmp_cmd->redirs)
		{
			tmp_redir = tmp_cmd->redirs;
			tmp_cmd->redirs = tmp_cmd->redirs->next;
			free(tmp_redir->file);
			free(tmp_redir);
		}
		free(tmp_cmd);
	}
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	t_token	*current_token;
	char	*expanded;

	if (!tokens)
		return (NULL);
	cmds = create_cmd();
	if (!cmds)
		return (NULL);
	current_cmd = cmds;
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == TOKEN_WORD)
		{
			expanded = expand_variables(current_token->value, env, 0);
			if (!expanded)
			{
				free_commands(cmds);
				return (NULL);
			}
			if (!add_arg(current_cmd, expanded))
			{
				free(expanded);
				free_commands(cmds);
				return (NULL);
			}
			free(expanded);
		}
		else if (current_token->type == TOKEN_PIPE)
		{
			current_cmd->next = create_cmd();
			if (!current_cmd->next)
			{
				free_commands(cmds);
				return (NULL);
			}
			current_cmd = current_cmd->next;
		}
		else if (current_token->type == TOKEN_REDIR_IN ||
				 current_token->type == TOKEN_REDIR_OUT ||
				 current_token->type == TOKEN_REDIR_APPEND ||
				 current_token->type == TOKEN_HEREDOC)
		{
			if (!current_token->next || current_token->next->type != TOKEN_WORD)
			{
				free_commands(cmds);
				return (NULL);
			}
			current_token = current_token->next;
			expanded = expand_variables(current_token->value, env, 0);
			if (!expanded)
			{
				free_commands(cmds);
				return (NULL);
			}
			if (!add_redir(current_cmd, current_token->type - 2, expanded))
			{
				free(expanded);
				free_commands(cmds);
				return (NULL);
			}
			free(expanded);
		}
		current_token = current_token->next;
	}
	return (cmds);
}
