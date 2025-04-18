/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:04:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 20:02:18 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "mod_syn.h"

static const char	*type_str(t_ntype type)
{
	static const char	*g_type_name[] = {"SIMPLE_CMD", "CMD", "PIPE", "LIST",
			"EOF", "AND", "OR", "SUBSHELL", "REDIRECT"};

	if (type < 0 || type >= (int)(sizeof(g_type_name) / sizeof(char *)))
		return ("UNKNOWN");
	return (g_type_name[type]);
}

static void	put_prefix(const char *prefix, int is_last)
{
	ft_putstr_fd((char *)prefix, 2);
	if (is_last)
		ft_putstr_fd("└── ", 2);
	else
		ft_putstr_fd("├── ", 2);
}

/* ****************************************************************** */
/*          コマンド / リダイレクト表示                                      */
/* ****************************************************************** */

static void	print_tok_list(t_list *lst, int redir)
{
	const char		*fmt[4] = {"%s", "< %s", "> %s", ">> %s"};
	int				first;
	t_lexical_token	*tok;

	first = 1;
	while (lst)
	{
		tok = lst->data;
		if (!first)
			ft_putstr_fd(", ", 2);
		if (!redir)
			ft_dprintf(2, fmt[0], tok->value);
		else if (tok->type == TT_REDIR_IN)
			ft_dprintf(2, fmt[1], tok->value);
		else if (tok->type == TT_REDIR_OUT)
			ft_dprintf(2, fmt[2], tok->value);
		else
			ft_dprintf(2, fmt[3], tok->value);
		first = 0;
		lst = lst->next;
	}
}

static void	print_cmd_detail(t_args *args, const char *pref, int is_last)
{
	char	*sub_pref;

	sub_pref = xstrjoin_free(ms_strdup(pref, NULL), is_last ? "    " : "│   ",
			NULL);
	if (args->argv)
	{
		put_prefix(sub_pref, 0);
		ft_putstr_fd("Command: [", 2);
		print_tok_list(args->argv, 0);
		ft_putendl_fd("]", 2);
	}
	if (args->redr)
	{
		put_prefix(sub_pref, 1);
		ft_putstr_fd("Redirs : [", 2);
		print_tok_list(args->redr, 1);
		ft_putendl_fd("]", 2);
	}
	free(sub_pref);
}

/* ****************************************************************** */
/*          木構造の再帰出力                                            */
/* ****************************************************************** */

static void	print_ast_rec(t_ast *ast, const char *pref, int is_last)
{
	char	*next_pref;

	if (!ast)
		return ;
	put_prefix(pref, is_last);
	ft_dprintf(2, "[%s]\n", type_str(ast->ntype));
	if (ast->ntype == NT_CMD)
		print_cmd_detail(ast->args, pref, is_last);
	/* 子ノードの prefix を作成 */
	next_pref = xstrjoin_free(ms_strdup(pref, NULL), is_last ? "    " : "│   ",
			NULL);
	if (ast->left && ast->right)
	{
		print_ast_rec(ast->left, next_pref, 0);
		print_ast_rec(ast->right, next_pref, 1);
	}
	else if (ast->left)
		print_ast_rec(ast->left, next_pref, 1);
	else if (ast->right)
		print_ast_rec(ast->right, next_pref, 1);
	free(next_pref);
}

/* ****************************************************************** */
/*          エントリポイント                                               */
/* ****************************************************************** */

void	debug_print_ast(t_ast *ast, t_shell *shell)
{
	(void)shell;
	if (!ast)
		ft_putendl_fd("(Empty AST)", 2);
	else
	{
		ft_putendl_fd("\n=== Abstract Syntax Tree ===", 2);
		print_ast_rec(ast, "", 1);
		ft_putendl_fd("===========================\n", 2);
	}
}
