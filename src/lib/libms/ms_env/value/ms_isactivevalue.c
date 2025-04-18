/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_isactivevalue.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 07:44:31 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 08:43:56 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/*
** 環境変数の値として使用できない特殊文字をチェックする関数
** 
** 返り値:
** 1: 値として使用可能
** 0: 値として使用不可能
**
** 以下の文字が含まれる場合は、エスケープが必要または使用不可:
** - NULL文字 (0x00)
** - 制御文字の一部
** - 特定のシェル特殊文字（エスケープなしで使用する場合）
*/
int	ms_isactivevalue(const char *value)
{
	size_t	i;

	if (!value)
		return (1);
	i = 0;
	if (ft_strchr(value, '$'))
	{
		while (value[i])
		{
			if (!ft_isalnum_under(value[i]) && value[i] != '$')
				return (0);
			i++;
		}
	}
	return (1);
}


