/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res_exec_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:53:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 21:08:46 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int proc_exec_path(t_list **lst, int idx, t_shell *sh)
{
	t_lexical_token	*data;
	char			*trimmed;

	data = (t_lexical_token *)(*lst)->data;
	if (sh->debug & DEBUG_SEM)
		ft_dprintf(STDERR_FILENO, "[proc_exec_path]: %s [POINTER]: %p\n",
			data->value, data->value);
	if (idx == 0)
	{
		trimmed = trim_valid_quotes(data->value, sh);
		data->value = trimmed;
		return (path_resolve(&data->value, sh));
	}
	return (0);
}
