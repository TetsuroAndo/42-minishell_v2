/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 03:14:30 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:14:31 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

extern volatile sig_atomic_t	g_signal_status;

static int	is_quotes_balanced(const char *line)
{
	int		single_open;
	int		double_open;
	size_t	pos;

	single_open = 0;
	double_open = 0;
	pos = 0;
	while (line[pos])
	{
		if (line[pos] == '#' && !single_open && !double_open)
			break ;
		if (line[pos] == '\'' && !double_open)
			single_open = !single_open;
		else if (line[pos] == '"' && !single_open)
			double_open = !double_open;
		pos++;
	}
	return (!single_open && !double_open);
}

char	*read_command_line(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
	{
		add_history(line);
	}
	return (line);
}

char	*launch_readline(const char *prompt)
{
	char	*line;
	char	*tmp;
	char	*cont_line;

	line = read_command_line(prompt);
	if (!line)
		return (NULL);
	while (!is_quotes_balanced(line))
	{
		if (g_signal_status == SIGINT)
			return (xfree((void **)&line), ft_strdup(""));
		cont_line = read_command_line("> ");
		if (!cont_line)
			return (xfree((void **)&line), ft_strdup(""));
		tmp = ft_strjoin_free(line, "\n");
		line = ft_strjoin_free2(tmp, cont_line);
		if (!line)
			return (NULL);
	}
	return (line);
}
