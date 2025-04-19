/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sh_proc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:24:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 20:48:31 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	put_sh_init(t_shell *sh)
{
	char	pid[36] = "This project does not use getpid().";

	ms_put_ascii(sh);
	ft_dprintf(STDERR_FILENO, "-----> [SHELL NAME]: %s\n", sh->bin_name);
	ft_dprintf(STDERR_FILENO, "[PID]: %s\n", pid);
}
