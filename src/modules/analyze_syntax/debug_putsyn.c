#include "mod_syn.h"
#include "core.h"
#include <stdio.h>

static void print_ast_node_type(t_ntype type)
{
	char *type_str;

	switch (type)
	{
	case NT_SIMPLE_CMD:
		type_str = "SIMPLE_CMD";
		break;
	case NT_CMD:
		type_str = "CMD";
		break;
	case NT_PIPE:
		type_str = "PIPE";
		break;
	case NT_EOF:
		type_str = "EOF";
		break;
	case NT_AND:
		type_str = "AND";
		break;
	case NT_OR:
		type_str = "OR";
		break;
	case NT_SUBSHELL:
		type_str = "SUBSHELL";
		break;
	case NT_REDIRECT:
		type_str = "REDIRECT";
		break;
	default:
		type_str = "UNKNOWN";
		break;
	}
	printf("%s", type_str);
}

static void print_args(t_args *args, int indent)
{
	int i;
	t_list *arg_list;
	t_list *redir_list;

	if (!args)
		return;

	// Print path if exists
	if (args->path[0] != '\0')
	{
		printf("%*sPath: %s\n", indent, "", args->path);
	}

	// Print arguments
	arg_list = args->argv;
	if (arg_list)
	{
		ft_printf("%*sArguments:\n", indent, "");
		i = 0;
		while (arg_list)
		{
			ft_printf("%*s  [%d]: %s\n", indent, "", i++, (char *)arg_list->data);
			arg_list = arg_list->next;
		}
	}

	// Print redirections
	redir_list = args->redr;
	if (redir_list)
	{
		ft_printf("%*sRedirections:\n", indent, "");
		i = 0;
		while (redir_list)
		{
			t_lexical_token *token = redir_list->data;
			ft_printf("%*s  [%d]: type=%#x, value=%s\n", indent, "", i++, 
				token->type, token->value);
			redir_list = redir_list->next;
		}
	}
}

static void print_ast_recursive(t_ast *ast, int indent)
{
	if (!ast)
		return;

	// Print current node
	ft_printf("%*sNode Type: ", indent, "");
	print_ast_node_type(ast->ntype);
	ft_printf("\n");

	// Print arguments if any
	if (ast->args)
	{
		ft_printf("%*sArgs:\n", indent, "");
		print_args(ast->args, indent + 2);
	}

	// Print children recursively with increased indentation
	if (ast->left)
	{
		ft_printf("%*sLeft Child:\n", indent, "");
		print_ast_recursive(ast->left, indent + 2);
	}

	if (ast->right)
	{
		ft_printf("%*sRight Child:\n", indent, "");
		print_ast_recursive(ast->right, indent + 2);
	}
}

void debug_print_ast(t_ast *ast)
{
	ft_printf("\n=== Abstract Syntax Tree ===\n");
	if (!ast)
	{
		ft_printf("(Empty AST)\n");
		return;
	}

	print_ast_recursive(ast, 0);
	ft_printf("===========================\n\n");
}
