/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_escapevalue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 07:46:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 07:56:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/*
** 環境変数の値に含まれる$をエスケープする関数
** 
** 返り値:
** エスケープされた新しい文字列（要解放）
** NULL: エラー発生時
*/
char	*ms_escapevalue(const char *value, t_shell *shell)
{
	char	*res;
	char	*pos;
	char	*tmp;
	char	*curr;

	if (!value)
		return (NULL);
	res = ms_strdup(value, shell);
	if (!res)
		return (NULL);
	curr = res;
	pos = ft_strpbrk(curr, "$");
	while (pos != NULL)
	{
		*pos = '\0';
		tmp = xstrjoin3(res, "\\$", pos + 1, shell);
		xfree((void **)&res);
		if (!tmp)
			return (NULL);
		res = tmp;
		curr = res + (pos - curr) + 2;
		pos = ft_strpbrk(curr, "$");
	}
	return (res);
}
