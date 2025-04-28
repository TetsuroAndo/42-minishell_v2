/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:56:24 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 03:04:13 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"
#include "ms_ast.h"

static void	clear_args(t_args *args)
{
	if (!args)
		return ;
	if (args->argv)
		ft_lstclear(&args->argv, free_token);
	if (args->redr)
		ft_lstclear(&args->redr, free_token);
	if (args->b_argv)
		ft_lstclear(&args->b_argv, free_token);
	if (args->b_redr)
		ft_lstclear(&args->b_redr, free_token);
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	if (args->pid != -1)
		waitpid(args->pid, NULL, 0);
	xfree((void **)&args);
}

void	free_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->left)
		free_ast(&(*ast)->left);
	if ((*ast)->right)
		free_ast(&(*ast)->right);
	if ((*ast)->args)
		clear_args((*ast)->args);
	xfree((void **)ast);
	*ast = NULL;
}
