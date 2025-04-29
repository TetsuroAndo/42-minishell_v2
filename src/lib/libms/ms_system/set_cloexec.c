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
#include <sys/ioctl.h>

void	set_cloexec_all(void)
{
	DIR				*d;
	struct dirent	*ent;
	int				fd;

	d = opendir("/proc/self/fd");
	if (!d)
		return ;
	while ((ent = readdir(d)))
	{
		fd = ft_atoi(ent->d_name);
		if (fd > 2 && fd != dirfd(d))
			ioctl(fd, FIOCLEX);
	}
	closedir(d);
}
