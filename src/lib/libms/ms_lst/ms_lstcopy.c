/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lstcopy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:59:22 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/25 21:50:25 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/**
 * @brief 要素を個別にコピーして新しいリストノードを作成する
 *
 * @param data 元のデータ
 * @param sh シェル情報
 * @return void* 新しくコピーされたデータ
 */
void	*copy_token(void *data, t_shell *sh)
{
	t_lexical_token	*src;
	t_lexical_token	*new;

	if (!data)
		return (NULL);
	src = data;
	new = xmalloc(sizeof(t_lexical_token), sh);
	if (!new)
		return (NULL);
	new->type = src->type;
	new->value = NULL;
	if (src->value)
		new->value = ms_strdup(src->value, sh);
	return (new);
}

/**
 * @brief リストをコピーする
 *
 * @param lst コピー元のリスト
 * @param del 解放関数ポインタ
 * @param shell シェル情報
 * @return t_list* 新しくコピーされたリスト
 */
t_list	*ms_lstcopy(t_list *lst, void (*del)(void *), t_shell *shell)
{
	t_list	*new_list;
	t_list	*node;
	void	*new_data;

	new_list = NULL;
	while (lst)
	{
		new_data = copy_token(lst->data, shell);
		if (!new_data)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		node = xlstnew(new_data, shell);
		if (!node)
		{
			if (del)
				del(new_data);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, node);
		lst = lst->next;
	}
	return (new_list);
}
