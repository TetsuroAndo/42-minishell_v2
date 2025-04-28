/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:07:23 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:21:28 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_READLINE_H
# define MS_READLINE_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>

char	*launch_readline(const char *prompt);
char	*read_command_line(const char *prompt);

#endif
