/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res_exec_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:53:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 19:43:19 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	proc_exec_path(t_list **lst, int idx, t_shell *sh)
{
	t_lexical_token	*data;
	char			*val;

	data = (t_lexical_token *)(*lst)->data;
	if (sh->debug & DEBUG_SEM)
		ft_dprintf(STDERR_FILENO, "[proc_exec_path]: %s [POINTER]: %p\n",
			data->value, data->value);
	if (idx == 0)
	{
		val = ms_strdup(data->value, sh);
		xfree((void **)&data->value);
		data->value = val;
		return (path_resolve(&data->value, sh));
	}
	return (0);
}
