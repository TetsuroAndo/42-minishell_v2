/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:04:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/15 15:24:18 by teando           ###   ########.fr       */
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

static void	print_cmd_args(t_args *args)
{
	t_list			*arg_list;
	t_lexical_token	*token;
	int				first;

	if (!args || !args->argv)
		return ;
	printf("[");
	arg_list = args->argv;
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

static void	print_tree_node(t_ast *ast, const char *prefix, int is_left)
{
	char *indent;
	char *path_str;
	
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
		if (is_left)
			indent = "│          ";
		else
			indent = "           ";
		// if (ast->args->path[0] != '\0')
		// 	path_str = ast->args->path;
		// else
		// 	path_str = "(no path)";
		// printf("\n%s%s├─ Path: %s", prefix, indent, path_str);
		printf("\n%s%s└─ Args: ", prefix, indent);
		print_cmd_args(ast->args);
		printf("\n");
	}
	else
		printf("\n");
}

static void	print_ast_tree(t_ast *ast, const char *prefix, int is_left, t_shell *shell)
{
	char	*new_prefix;

	if (!ast)
		return ;
	print_tree_node(ast, prefix, is_left);
	if (is_left)
		new_prefix = xstrjoin_free(ms_strdup(prefix, shell), "│          ", shell);
	else
		new_prefix = xstrjoin_free(ms_strdup(prefix, shell), "           ", shell);
	if (ast->left || ast->right)
	{
		if (ast->left)
			print_ast_tree(ast->left, new_prefix, 1, shell);
		if (ast->right)
			print_ast_tree(ast->right, new_prefix, 0, shell);
	}
	xfree((void **)&new_prefix);
}

void	debug_print_ast(t_ast *ast, t_shell *shell)
{
	if (!ast || (ast->left == NULL && ast->right == NULL))
	{
		printf("(Empty AST)\n");
		return ;
	}
	printf("\n=== Abstract Syntax Tree ===\n");
	printf("[%s]\n", get_node_type_str(ast->ntype));
	if (ast->left)
		print_ast_tree(ast->left, "", 1, shell);
	if (ast->right)
		print_ast_tree(ast->right, "", 0, shell);
	printf("===========================\n");
}
		