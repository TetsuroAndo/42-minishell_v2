/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:45:55 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:29:11 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

static void	put_indent(int depth)
{
	while (depth--)
		ft_dprintf(2, "  ");
}

static void	print_toklst(t_list *lst, char *lbl, int depth)
{
	if (!lst)
		return ;
	put_indent(depth);
	ft_dprintf(2, "%s:", lbl);
	while (lst)
	{
		ft_dprintf(2, " '%s'", ((t_lexical_token *)lst->data)->value);
		lst = lst->next;
	}
	ft_dprintf(2, "\n");
}

static const char	*ntype_name(int n)
{
	if (n == NT_CMD)
		return ("CMD");
	if (n == NT_PIPE)
		return ("PIPE");
	if (n == NT_AND)
		return ("AND");
	if (n == NT_OR)
		return ("OR");
	if (n == NT_LIST)
		return ("LIST");
	return ("UNK");
}

static void	print_ast_rec(t_ast *ast, int depth)
{
	if (!ast)
		return ;
	put_indent(depth);
	ft_dprintf(2, "[%s]\n", ntype_name(ast->ntype));
	if (ast->ntype == NT_CMD)
	{
		print_toklst(ast->args->argv, "argv", depth + 1);
		print_toklst(ast->args->redr, "redir", depth + 1);
	}
	print_ast_rec(ast->left, depth + 1);
	print_ast_rec(ast->right, depth + 1);
}

void	debug_print_sem(t_ast *ast, t_shell *sh)
{
	(void)sh;
	ft_dprintf(2, "===== semantic dump =====\n");
	print_ast_rec(ast, 0);
	ft_dprintf(2, "=========================\n");
}
