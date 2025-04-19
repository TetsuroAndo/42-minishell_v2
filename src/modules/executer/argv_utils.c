/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:48:51 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 22:48:51 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

/* トークンリストを NULL 終端 argv[] に変換する */
char	**toklist_to_argv(t_list *lst, t_shell *sh)
{
	t_lexical_token	*tok;
	char			**argv;
	size_t			n;
	size_t			i;

	n = ft_lstsize(lst);
	argv = xmalloc_gcline(sizeof(char *) * (n + 1), sh);
	i = 0;
	while (lst)
	{
		tok = lst->data;
		argv[i++] = tok->value;
		lst = lst->next;
	}
	argv[i] = NULL;
	return (argv);
}
