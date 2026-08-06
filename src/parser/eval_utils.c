/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberome <alberome@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 18:40:08 by alberome          #+#    #+#             */
/*   Updated: 2026/03/25 21:52:08 by alberome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_segment(char *segment)
{
	size_t	l;
	size_t	r;

	l = 0;
	while (segment[l] == ' ' || segment[l] == '\t')
		++l;
	r = ft_strlen(segment);
	while (r > l && (segment[r - 1] == ' ' || segment[r - 1] == '\t'))
		--r;
	if (l > 0)
		ft_memcpy(segment, segment + l, r - l);
	segment[r - l] = '\0';
	return (segment);
}

char	*extract_parenthesized(const char *s)
{
	size_t	i;
	int		quote;
	size_t	depth;

	if (!s || *s != '(')
		return (NULL);
	i = 1;
	depth = 1;
	quote = 0;
	while (s[i])
	{
		if ((s[i] == '"' || s[i] == '\'') && !quote)
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else if (!quote && s[i] == '(')
			++depth;
		else if (!quote && s[i] == ')' && --depth == 0)
			break ;
		++i;
	}
	if (s[i] != ')' || s[i + 1] != '\0')
		return (NULL);
	return (ft_strndup(s + 1, i - 1));
}

int	run_eval_segment(t_shell *sh, char *seg)
{
	char		*inner;
	t_pipeline	pl;
	int			status;

	inner = NULL;
	if (seg && seg[0] == '(')
		inner = extract_parenthesized(seg);
	if (inner)
		return (status = eval_subshell(sh, inner), free(inner), status);
	if (!seg || !*seg)
		return (0);
	pl = (t_pipeline){0};
	if (!parse_line(sh, seg, &pl))
		return (0);
	status = 1;
	if (run_heredocs(sh, &pl) == 0)
		status = exec_pipeline(sh, &pl);
	free_pipeline(&pl);
	return (status);
}

static int	wait_child(pid_t pid)
{
	int	st;
	int	status;

	st = 0;
	status = 1;
	if (waitpid(pid, &st, 0) < 0)
		ft_print_perror("waitpid");
	else if (WIFEXITED(st))
		status = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
		status = 128 + WTERMSIG(st);
	return (status);
}

int	eval_subshell(t_shell *sh, char *inner)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid < 0)
	{
		ft_print_perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		child_status = 0;
		eval_line(sh, inner, &child_status);
		_exit(child_status);
	}
	return (wait_child(pid));
}
