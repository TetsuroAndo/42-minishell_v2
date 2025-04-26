/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_err.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/26 19:02:25 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MS_ES_H
# define MS_ES_H

/*
** ==========================================================================
**  Minishell – unified error messages & exit status codes
**  (extended for Bash‑compatible behaviour)
** ==========================================================================
*/

/*--------------------------------------------------------------------------*/
/*  Generic printf‑style formatter                                          */
/*--------------------------------------------------------------------------*/
# define EMSG1 "minishell: %s\n"
# define EMSG2 "minishell: %s: %s\n"
# define EMSG3 "minishell: %s: %s: %s\n"

/*--------------------------------------------------------------------------*/
/*  Message literals (LANG=C originals)                                     */
/*--------------------------------------------------------------------------*/
# define ES_TOKEN "minishell: syntax error near unexpected token\n"
# define ES_NEWLINE "minishell: syntax error near unexpected token 'newline'\n"
# define ES_UNEXPECTED_EOF "minishell: unexpected EOF while looking for matching quote\n"
# define ES_PERMISSION "minishell: %s: permission denied\n"
# define ES_NO_SUCH_FOD "minishell: %s: No such file or directory\n"
# define ES_IS_DIR "minishell: %s: Is a directory\n"
# define ES_IS_FILE "Is a regular file"
# define ES_NOT_DIR "Not a directory"
# define ES_NOT_EXEC_FILE "cannot execute binary file: Exec format error"
# define ES_TEXT_BUSY "Text file busy"
# define ES_CMD_NOT_FOUND "minishell: %s: command not found\n"
# define ES_AMBIGUOUS "ambiguous redirect"
# define ES_NUMERIC "numeric argument required"
# define ES_TOO_MANY_ARGS "too many arguments"
# define ES_NAVI "not a valid identifier"
# define ES_SHIFT_RANGE "shift count out of range"
# define ES_BAD_SUBSTITUTION "bad substitution"
# define ES_BROKEN_PIPE "Broken pipe"
# define ES_EVENT_NOT_FOUND "event not found"
# define ES_WRITE_BROKEN_PIPE "write error: Broken pipe"
# define ES_HEREDOC "minishell: warning: here-document delimited by end-of-file\n"

/*
**--------------------------------------------------------------------------
**  Exit‑status map (duplicate numeric values are intentional
**  they make code more readable while staying Bash‑compatible)
**--------------------------------------------------------------------------
*/
typedef enum e_status
{
	/* Success */
	E_NONE = 0,

	/* Generic errors */
	E_GENERAL = 1,
	E_SYSTEM = 1,
	E_ENV_KEY = 1,
	E_PIPE = 1,
	E_FILE = 1,
	E_ARGUMENT = 1,
	E_INVALID_IDENTIFIER = 1,
	E_AMBIGUOUS_REDIR = 1,

	/* Misuse of shell built‑ins (Bash returns 2) */
	E_MISUSE_BUILTIN = 2,
	E_SYNTAX = 2,
	E_NUMERIC = 2,

	/* Non‑fatal: command name recognised but not executed */
	E_IS_DIR = 126,
	E_IS_FILE = 126,
	E_NOT_DIR = 126,
	E_NOT_FILE = 126,
	E_PERMISSION_DENIED = 126,
	E_NOT_EXECUTABLE = 126,
	E_EXEC_CANNOT_EXECUTE = 126,

	/* Name lookup failure */
	E_NO_SUCH_FOD = 127,
	E_COMMAND_NOT_FOUND = 127,

	/* INVALID EXIT ARGUMENT = n % 128 */
	E_INVALID_ARGUMENT = 128,

	/* Signals (128 + signo) */
	E_SIGINT = 130,  /* 128 + 2, SIGINT */
	E_SIGQUIT = 131, /* 128 + 3, SIGQUIT */
	E_SIGPIPE = 141, /* 128 + 13, SIGPIPE */

	/* Fatal / implementation‑specific */
	E_FATAL = 255,

	/* Project‑specific helpers */
	E_NOT_BUILTIN_CMD = -2
}	t_status;

#endif
