/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree_insert_data.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:32:09 by teando            #+#    #+#             */
/*   Updated: 2024/12/17 08:23:12 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_btree.h"

// t_btree	*ft_btree_create_node(void *item);

void	ft_btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *,
			void *))
{
	if (!root)
		return ;
	if (*root == 0)
	{
		*root = ft_btree_create_node(item);
		return ;
	}
	if (cmpf(item, (*root)->item) < 0)
		ft_btree_insert_data(&((*root)->left), item, cmpf);
	else
		ft_btree_insert_data(&((*root)->right), item, cmpf);
}
