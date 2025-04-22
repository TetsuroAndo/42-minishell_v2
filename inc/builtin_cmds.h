/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:59:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 15:57:38 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_CMDS_H
# define BUILTIN_CMDS_H

# include "core.h"

struct			s_bimapping
{
	const char	*name;
	t_status	(*fn)(int, char **, t_shell *);
};

t_status		__cd(int argc, char **argv, t_shell *sh);
t_status		__pwd(int argc, char **argv, t_shell *sh);
t_status		__export(int argc, char **argv, t_shell *sh);
t_status		__unset(int argc, char **argv, t_shell *sh);
t_status		__env(int argc, char **argv, t_shell *sh);
t_status		__echo(int argc, char **argv, t_shell *sh);
t_status		__exit(int argc, char **argv, t_shell *sh);
t_status		__true(int argc, char **argv, t_shell *sh);

/* dispatcher */
int				path_launcher(char path[], const char *src, int mode, t_shell *sh);
int				builtin_launch(char **argv, t_shell *sh);

#endif
