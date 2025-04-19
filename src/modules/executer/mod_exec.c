/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:33:11 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 05:57:21 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

/* ========================================================= */
/*                   Signal handling utility                 */
/* ========================================================= */

void	sig_ignore_parent(int *enabled)
{
	static struct sigaction	old_int, old_quit;
	struct sigaction		ign;

	if (*enabled)			/* restore */
	{
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		*enabled = 0;
		return ;
	}
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;

	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	*enabled = 1;
}

/* ──────── static forward decls ──────── */
static int	exe_cmd(t_ast *node, t_shell *sh);
static int	exe_pipe(t_ast *node, t_shell *sh);
static int	exe_bool(t_ast *node, t_shell *sh); /* AND / OR / LIST */
static int	exe_sub(t_ast *node, t_shell *sh);
static int	handle_redr(t_args *args, t_shell *sh);

static void	fdbackup_enter(t_fdbackup *bk, int tgt, t_shell *sh)
{
	bk->target_fd = tgt;
	bk->saved_fd = xdup(tgt, sh);
}
static void	fdbackupexit(t_fdbackup *bk)
{
	if (bk->saved_fd != -1)
	{
		dup2(bk->saved_fd, bk->target_fd);
		close(bk->saved_fd);
	}
}

/* ========================================================= */
/*                   AST dispatcher : exe_run                */
/* ========================================================= */

int	exe_run(t_ast *node, t_shell *sh)
{
	if (!node)
		return (0);
	else if (node->ntype == NT_CMD)
		return (exe_cmd(node, sh));
	else if (node->ntype == NT_PIPE)
		return (exe_pipe(node, sh));
	else if (node->ntype == NT_AND || node->ntype == NT_OR
		|| node->ntype == NT_LIST)
		return (exe_bool(node, sh));
	else if (node->ntype == NT_SUBSHELL)
		return (exe_sub(node, sh));
	else
		return (1); /* 不明ノード型 → エラー */
}

/* ========================================================= */
/*                        NT_CMD                             */
/* ========================================================= */

static int	exe_cmd(t_ast *node, t_shell *sh)
{
	char		**argv;
	t_fdbackup	bk_in;
	t_fdbackup	bk_out;
	int			status;
	pid_t		pid;
	int			wstatus;
	struct stat	sb;
	int			sig_held;

	if (!node || node->ntype != NT_CMD)
		return (1);
	/* 1. リダイレクトを解析して FD を args->fds へ */
	if (handle_redr(node->args, sh))
		return (1);
	/* 2. argv を C 配列化 */
	argv = toklist_to_argv(node->args->argv, sh);
	if (!argv || !argv[0])
		return (127);
	if (stat(argv[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		return (E_IS_DIR);
	/* 3. FD 差し替え */
	bk_in = (t_fdbackup){-1, STDIN_FILENO};
	bk_out = (t_fdbackup){-1, STDOUT_FILENO};
	if (node->args->fds[0] != -1)
	{
		fdbackup_enter(&bk_in, STDIN_FILENO, sh);
		xdup2(&node->args->fds[0], STDIN_FILENO, sh);
	}
	if (node->args->fds[1] != -1)
	{
		fdbackup_enter(&bk_out, STDOUT_FILENO, sh);
		xdup2(&node->args->fds[1], STDOUT_FILENO, sh);
	}
	/* 4. ビルトイン判定 → 実行 */
	if (is_builtin(argv[0]))
		status = builtin_launch(argv, sh);
	else
	{
		sig_held = 0;
		sig_ignore_parent(&sig_held);	/* ── ① 親は一時的に無視 ── */
		pid = xfork(sh);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execvp(argv[0], argv);
			perror(argv[0]);
			exit(127);
		}
		node->args->pid = pid;
		waitpid(pid, &wstatus, 0);
		sig_ignore_parent(&sig_held);	/* ── ② 親ハンドラを復元 ── */
		if (WIFEXITED(wstatus))
			status = WEXITSTATUS(wstatus);
		else
			status = 128 + WTERMSIG(wstatus);
	}
	/* 5. FD 復旧 & 後始末 */
	fdbackupexit(&bk_in);
	fdbackupexit(&bk_out);
	return (status);
}

/* ========================================================= */
/*                         NT_PIPE                           */
/* ========================================================= */

static int	exe_pipe(t_ast *node, t_shell *sh)
{
	int		fds[2];
	pid_t	lpid;
	int		st;
	pid_t	rpid;
	int		st_l;
	int		st_r;
	int		sig_held;

	sig_held = 0;
	sig_ignore_parent(&sig_held);	/* ── ① 親は一時的に無視 ── */
	xpipe(fds, sh); /* fds[0] = r, fds[1] = w */
	lpid = xfork(sh);
	if (lpid == 0)
	{
		xdup2(&fds[1], STDOUT_FILENO, sh);
		close(fds[0]);
		close(fds[1]);
		st = exe_run(node->left, sh);
		exit(st);
	}
	rpid = xfork(sh);
	if (rpid == 0)
	{
		xdup2(&fds[0], STDIN_FILENO, sh);
		close(fds[0]);
		close(fds[1]);
		st = exe_run(node->right, sh);
		exit(st);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(lpid, &st_l, 0);
	waitpid(rpid, &st_r, 0);
	sig_ignore_parent(&sig_held);	/* ── ② 親ハンドラを復元 ── */
	if (WIFEXITED(st_r))
		return (WEXITSTATUS(st_r));
	return (128 + WTERMSIG(st_r));
}

/* ========================================================= */
/*                  NT_AND / NT_OR / NT_LIST                  */
/* ========================================================= */

static int	exe_bool(t_ast *node, t_shell *sh)
{
	int	st_left;
	int	run_right;

	st_left = exe_run(node->left, sh);
	run_right = 0;
	if (node->ntype == NT_AND && st_left == 0)
		run_right = 1;
	if (node->ntype == NT_OR && st_left != 0)
		run_right = 1;
	if (node->ntype == NT_LIST)
		run_right = 1;
	if (run_right)
		return (exe_run(node->right, sh));
	return (st_left);
}

/* ========================================================= */
/*                        NT_SUBSHELL                        */
/* ========================================================= */

static int	exe_sub(t_ast *node, t_shell *sh)
{
	pid_t	pid;
	int		wst;

	pid = xfork(sh);
	if (pid == 0)
	{
		int st = exe_run(node->left, sh); /* subshell の AST は left に格納 */
		exit(st);
	}
	waitpid(pid, &wst, 0);
	if (WIFEXITED(wst))
		return (WEXITSTATUS(wst));
	return (128 + WTERMSIG(wst));
}

/* ========================================================= */
/*                  ★ 追加：ヒアドキュメント ★                   */
/* ========================================================= */
static int	heredoc_into_fd(char *body, t_args *args, t_shell *sh)
{
	int	hd[2];

	if (xpipe(hd, sh))
		return (1);
	/* write() は短い本文なら一括送信で十分。失敗時はシェルごと死ぬ想定。*/
	if (write(hd[1], body, ft_strlen(body)) == -1 || close(hd[1]) == -1)
	{
		perror("heredoc write");
		close(hd[0]);
		return (1);
	}
	/* 直近の < や << が勝つ POSIX 仕様を踏襲。 */
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	args->fds[0] = hd[0]; /* read end を stdin 置換候補に */
	return (0);
}

/* ========================================================= */
/*                 ★ 改修：redirect 一括ハンドラ ★              */
/* ========================================================= */
static int	handle_redr(t_args *args, t_shell *sh)
{
	t_list			*lst;
	t_lexical_token	*tok;

	lst = args->redr;
	while (lst)
	{
		tok = lst->data;
		/*
		** 1) ここで ttype を見て FD を確定。
		** 2) << は SEM フェーズで TT_REDIR_IN へ変換されるので
		**    "値に改行が含まれるか" でヒアドキュメントと判別。
		*/
		if (tok->type == TT_REDIR_IN)
		{
			if (ft_strchr(tok->value, '\n')) /* here‑doc 本文 */
			{
				if (heredoc_into_fd(tok->value, args, sh))
					return (1);
			}
			else
			{
				if (args->fds[0] > 2)
					xclose(&args->fds[0]);
				args->fds[0] = open(tok->value, O_RDONLY);
			}
		}
		else if (tok->type == TT_REDIR_OUT)
		{
			if (args->fds[1] > 2)
				xclose(&args->fds[1]);
			args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (tok->type == TT_APPEND)
		{
			if (args->fds[1] > 2)
				xclose(&args->fds[1]);
			args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		}
		if ((args->fds[0] == -1 && tok->type == TT_REDIR_IN) || (args->fds[1] ==
				-1 && (tok->type == TT_REDIR_OUT || tok->type == TT_APPEND)))
			return (perror(tok->value), 1);
		lst = lst->next;
	}
	return (0);
}
