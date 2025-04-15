#include "core.h"
#include "mod_syn.h"
#include <stdio.h>

static void	print_ast_node_type(t_ntype type)
{
	char	*type_str;

	switch (type)
	{
	case NT_SIMPLE_CMD:
		type_str = "SIMPLE_CMD";
		break ;
	case NT_CMD:
		type_str = "CMD";
		break ;
	case NT_PIPE:
		type_str = "PIPE";
		break ;
	case NT_EOF:
		type_str = "EOF";
		break ;
	case NT_AND:
		type_str = "AND";
		break ;
	case NT_OR:
		type_str = "OR";
		break ;
	case NT_SUBSHELL:
		type_str = "SUBSHELL";
		break ;
	case NT_REDIRECT:
		type_str = "REDIRECT";
		break ;
	case NT_LIST:
		type_str = "LIST";
		break ;
	default:
		type_str = "UNKNOWN";
		break ;
	}
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
			printf("%*s  [%d]: %s\n", indent, "", i++, ((t_lexical_token *)arg_list->data)->value);
			arg_list = arg_list->next;
		}
	}
	// Print redirections
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

void	debug_print_ast(t_ast *ast)
{
	printf("\n=== Abstract Syntax Tree ===\n");
	if (!ast)
	{
		printf("(Empty AST)\n");
		return ;
	}
	print_ast_recursive(ast, 0);
	printf("===========================\n\n");
}
