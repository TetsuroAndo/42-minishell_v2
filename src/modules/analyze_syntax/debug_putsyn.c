/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putsyn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:04:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/15 13:53:22 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "core.h"
#include "mod_syn.h"
#include <stdio.h>

static void	print_ast_node_type(t_ntype type)
{
	char				*type_str;
	static const char	*type_names[] = {"SIMPLE_CMD", "CMD", "PIPE", "EOF",
			"AND", "OR", "SUBSHELL", "REDIRECT"};

	if (type < 0 || type >= sizeof(type_names) / sizeof(type_names[0]))
		type_str = "UNKNOWN";
	else
		type_str = (char *)type_names[type];
	printf("%s", type_str);
}

static void	print_args(t_args *args, int indent)
{
	int				i;
	t_list			*arg_list;
	t_list			*redir_list;
	t_lexical_token	*token;

	if (!args)
		return ;
	// Print path if exists
	if (args->path[0] != '\0')
	{
		printf("%*sPath: %s\n", indent, "", args->path);
	}
	// Print arguments
	arg_list = args->argv;
	if (arg_list)
	{
		printf("%*sArguments:\n", indent, "");
		i = 0;
		while (arg_list)
		{
			printf("%*s  [%d]: %s\n", indent, "", i++,
				((t_lexical_token *)arg_list->data)->value);
			arg_list = arg_list->next;
		}
	}
	redir_list = args->redr;
	if (redir_list)
	{
		printf("%*sRedirections:\n", indent, "");
		i = 0;
		while (redir_list)
		{
			token = redir_list->data;
			printf("%*s  [%d]: type=%#x, value=%s\n", indent, "", i++,
				token->type, token->value);
			redir_list = redir_list->next;
		}
	}
}

static void	print_ast_recursive(t_ast *ast, int indent)
{
	if (!ast)
		return ;
	// Print current node
	printf("%*sNode Type: ", indent, "");
	print_ast_node_type(ast->ntype);
	printf("\n");
	// Print arguments if any
	if (ast->args)
	{
		printf("%*sArgs:\n", indent, "");
		print_args(ast->args, indent + 2);
	}
	// Print children recursively with increased indentation
	if (ast->left)
	{
		printf("%*sLeft Child:\n", indent, "");
		print_ast_recursive(ast->left, indent + 2);
	}
	if (ast->right)
	{
		printf("%*sRight Child:\n", indent, "");
		print_ast_recursive(ast->right, indent + 2);
	}
}

static void	print_args_visual(t_args *args, int depth, const char *prefix)
{
	t_list			*arg_list;
	int				i;
	t_list			*redir_list;
	t_lexical_token	*token;

	if (!args)
		return ;
	// Print path if exists
	if (args->path[0] != '\0')
	{
		printf("%s    ├─ Path: %s\n", prefix, args->path);
	}
	// Print arguments
	arg_list = args->argv;
	if (arg_list)
	{
		printf("%s    ├─ Arguments:\n", prefix);
		i = 0;
		while (arg_list)
		{
			printf("%s    │   ├─ [%d]: %s\n", prefix, i++,
				((t_lexical_token *)arg_list->data)->value);
			arg_list = arg_list->next;
		}
	}
	// Print redirections
	redir_list = args->redr;
	if (redir_list)
	{
		printf("%s   └── Redirections:\n", prefix);
		i = 0;
		while (redir_list)
		{
			token = redir_list->data;
			printf("%s      ├─ [%d]: type=%#x, value=%s\n", prefix, i++,
				token->type, token->value);
			redir_list = redir_list->next;
		}
	}
}

static void	print_ast_recursive_visual(t_ast *ast, int depth,
		const char *prefix)
{
	char	new_prefix[256];
	char	last_prefix[256];

	if (!ast)
		return ;
	// Print current node
	printf("%s", prefix);
	printf("└── ");
	print_ast_node_type(ast->ntype);
	printf("\n");
	// Print arguments if any
	if (ast->args)
	{
		printf("%s    Args:\n", prefix);
		print_args_visual(ast->args, depth + 1, prefix);
	}
	// Prepare new prefix for children
	snprintf(new_prefix, sizeof(new_prefix), "%s     │", prefix);
	// Print children recursively
	if (ast->left)
	{
		print_ast_recursive_visual(ast->left, depth + 1, new_prefix);
	}
	if (ast->right)
	{
		// For the last child, use a different prefix
		snprintf(last_prefix, sizeof(last_prefix), "%s     ", prefix);
		print_ast_recursive_visual(ast->right, depth + 1, last_prefix);
	}
}

void	debug_print_ast(t_ast *ast)
{
	printf("\n=== Abstract Syntax Tree ===\n");
	if (!ast)
	{
		printf("(Empty AST)\n");
		return ;
	}
	// print_ast_recursive(ast, 0);
	print_ast_recursive_visual(ast, 0, "");
	printf("===========================\n\n");
}
