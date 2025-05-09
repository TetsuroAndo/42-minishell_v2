/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:04:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:22:23 by teando           ###   ########.fr       */
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

	if (is_last)
		sub_pref = xstrjoin_free(ms_strdup(pref, NULL), "    ", NULL);
	else
		sub_pref = xstrjoin_free(ms_strdup(pref, NULL), "│   ", NULL);
	if (args->argv)
	{
		ft_putstr_fd((char *)sub_pref, 2);
		ft_putstr_fd("├── Command: [", 2);
		print_tok_list(args->argv, 0);
		ft_putendl_fd("]", 2);
	}
	if (args->redr)
	{
		ft_putstr_fd((char *)sub_pref, 2);
		ft_putstr_fd("└── Redirs : [", 2);
		print_tok_list(args->redr, 1);
		ft_putendl_fd("]", 2);
	}
	xfree((void **)&sub_pref);
}

static void	print_ast_rec(t_ast *ast, const char *pref, int is_last)
{
	char	*next_pref;

	ft_putstr_fd((char *)pref, 2);
	if (is_last)
		ft_putstr_fd("└── ", 2);
	else
		ft_putstr_fd("├── ", 2);
	ft_dprintf(2, "[%s]\n", type_str(ast->ntype));
	if (ast->ntype == NT_CMD)
		print_cmd_detail(ast->args, pref, is_last);
	if (is_last)
		next_pref = xstrjoin_free(ms_strdup(pref, NULL), "    ", NULL);
	else
		next_pref = xstrjoin_free(ms_strdup(pref, NULL), "│   ", NULL);
	if (ast->left && ast->right)
	{
		print_ast_rec(ast->left, next_pref, 0);
		print_ast_rec(ast->right, next_pref, 1);
	}
	else if (ast->left)
		print_ast_rec(ast->left, next_pref, 1);
	else if (ast->right)
		print_ast_rec(ast->right, next_pref, 1);
	xfree((void **)&next_pref);
}

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
