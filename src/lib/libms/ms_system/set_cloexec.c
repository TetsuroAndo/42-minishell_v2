/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cloexec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:08:35 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 20:02:29 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"
#include <dirent.h>

void	set_cloexec_all(void)
{
	DIR				*d;
	struct dirent	*ent;
	int				fd;

	d = opendir("/proc/self/fd");
	if (!d)
		return ;
	ent = readdir(d);
	while (ent)
	{
		fd = ft_atoi(ent->d_name);
		if (fd > 2)
			fcntl(fd, F_SETFD, FD_CLOEXEC);
		ent = readdir(d);
	}
	closedir(d);
}
