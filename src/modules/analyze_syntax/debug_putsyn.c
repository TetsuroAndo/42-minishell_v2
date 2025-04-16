/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:04:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/16 16:30:04 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "mod_syn.h"

static char	*get_node_type_str(t_ntype type)
{
	static const char	*type_names[] = {
		"SIMPLE_CMD",
		"CMD",
		"PIPE",
		"LIST",
		"EOF",
		"AND",
		"OR",
		"SUBSHELL",
		"REDIRECT"
	};

	if (type < 0 || type >= sizeof(type_names) / sizeof(type_names[0]))
		return ("UNKNOWN");
	return ((char *)type_names[type]);
}

static void	print_cmd_args(t_args *args, int index)
{
	t_list			*arg_list;
	t_lexical_token	*token;
	int				first;

	if (!args || !args->argv)
		return ;
	printf("[");
	if (index == 0)
		arg_list = args->argv;
	else
		arg_list = args->redr;
	first = 1;
	while (arg_list)
	{
		token = arg_list->data;
		if (!first)
			printf(", ");
		printf("\"%s\"", token->value);
		first = 0;
		arg_list = arg_list->next;
	}
	printf("]");
}

static void	print_tree_node(t_ast *ast, const char *prefix, int is_left,
		int has_right)
{
	char	*indent;

	if (!ast)
		return ;
	printf("%s", prefix);
	if (is_left)
		printf("├─ LEFT ── ");
	else
		printf("└─ RIGHT ─ ");
	printf("[%s]", get_node_type_str(ast->ntype));
	if (ast->ntype == NT_CMD && ast->args)
	{
		indent = has_right ? "│          " : "           ";
		printf("\n%s%s", prefix, indent);
		if (ast->args->redr)
			printf("├─ Command: ");
		else
			printf("└─ Args: ");
		print_cmd_args(ast->args, 0);
		if (ast->args->redr)
		{
			printf("\n%s%s└─ Redirs: ", prefix, indent);
			print_cmd_args(ast->args, 1);
		}
		printf("\n");
	}
	else
		printf("\n");
}

static void	print_ast_tree(t_ast *ast, const char *prefix, int is_left,
		t_shell *shell)
{
	char	*new_prefix;

	if (!ast)
		return ;
	print_tree_node(ast, prefix, is_left, ast->right != NULL);
	if (is_left && ast->right)
		new_prefix = xstrjoin_free(ms_strdup(prefix, shell), "│  ", shell);
	else
		new_prefix = xstrjoin_free(ms_strdup(prefix, shell), "    ", shell);
	if (ast->left)
		print_ast_tree(ast->left, new_prefix, 1, shell);
	if (ast->right)
		print_ast_tree(ast->right, new_prefix, 0, shell);
	xfree((void **)&new_prefix);
}

void	debug_print_ast(t_ast *ast, t_shell *shell)
{
	printf("\n=== Abstract Syntax Tree ===\n");
	if (!ast)
		printf("(Empty AST)\n");
	else
		print_ast_tree(ast, "", 1, shell);
	printf("===========================\n\n");
}