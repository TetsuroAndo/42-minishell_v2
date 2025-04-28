#include "mod_exec.h"

/*******************************************************************************
 * FD-backup ユーティリティ
 ******************************************************************************/

static void fdbackup_enter(t_fdbackup *bk, int tgt, t_shell *sh)
{
    bk->target = tgt;
    bk->saved  = xdup(tgt, sh);
}

static void fdbackup_exit(t_fdbackup *bk)
{
    if (bk->saved != -1)
    {
        dup2(bk->saved, bk->target);
        close(bk->saved);
    }
}

/*******************************************************************************
 * シグナル一時無効化 (親プロセス用)
 ******************************************************************************/

static void toggle_sig_ignore(int *held)
{
    static struct sigaction  old_int;
    static struct sigaction  old_quit;
    struct sigaction         ign;

    if (*held)
    {
        sigaction(SIGINT, &old_int,  NULL);
        sigaction(SIGQUIT, &old_quit, NULL);
        *held = 0;
        return ;
    }
    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    ign.sa_flags = 0;
    sigaction(SIGINT,  &ign, &old_int);
    sigaction(SIGQUIT, &ign, &old_quit);
    *held = 1;
}

/*******************************************************************************
 * 共通ヘルパ : waitpid → exit status 変換
 ******************************************************************************/

static int wait_and_status(pid_t pid)
{
    int wst;

    if (waitpid(pid, &wst, 0) == -1)
        return (E_SYSTEM);
    if (WIFEXITED(wst))
        return (WEXITSTATUS(wst));
    return (128 + WTERMSIG(wst));
}

/*******************************************************************************
 * リダイレクト共通ルーチン
 ******************************************************************************/

static int  open_fd(const char *path, int flags)
{
#ifdef O_CLOEXEC
    int fd = open(path, flags | O_CLOEXEC, 0644);
#else
    int fd = open(path, flags, 0644);
#endif
    if (fd == -1)
        perror(path);
    return (fd);
}

static int apply_input_redir(t_lexical_token *tok, t_args *a, t_shell *sh)
{
    if (tok->type == TT_HEREDOC)
        return (heredoc_into_fd(tok->value, a, sh));
    if (tok->value && tok->value[0] == '\0')
        return ((a->fds[0] = open_fd("/dev/null", O_RDONLY)) == -1);
    return ((a->fds[0] = open_fd(tok->value, O_RDONLY)) == -1);
}

static int apply_output_redir(t_lexical_token *tok, t_args *a, int mode)
{
    return ((a->fds[1] = open_fd(tok->value, mode)) == -1);
}

static int handle_redirections(t_args *args, t_shell *sh)
{
    t_list          *lst = args->redr;
    t_lexical_token *tok;

    while (lst)
    {
        tok = lst->data;
        if (proc_redr_errs(tok, sh))
            return (1);
        if (tok->type == TT_REDIR_IN || tok->type == TT_HEREDOC)
        {
            if (apply_input_redir(tok, args, sh))
                return (1);
        }
        else if (tok->type == TT_REDIR_OUT)
        {
            if (apply_output_redir(tok, args, O_WRONLY | O_CREAT | O_TRUNC))
                return (1);
        }
        else if (tok->type == TT_APPEND)
        {
            if (apply_output_redir(tok, args, O_WRONLY | O_CREAT | O_APPEND))
                return (1);
        }
        lst = lst->next;
    }
    return (0);
}

/*******************************************************************************
 * redirection fd 後片付け
 ******************************************************************************/

static void cleanup_redir_fds(t_args *a)
{
    if (a->fds[0] > 2)
    {
        close(a->fds[0]);
        a->fds[0] = -1;
    }
    if (a->fds[1] > 2)
    {
        close(a->fds[1]);
        a->fds[1] = -1;
    }
}

/*******************************************************************************
 * 外部コマンド実行
 ******************************************************************************/

static int exec_external(char **argv, t_ast *n, t_shell *sh)
{
    int     sig_held = 0;
    char    **env;

    toggle_sig_ignore(&sig_held);
    n->args->pid = xfork(sh);
    if (n->args->pid == 0)
    {
        signal(SIGINT,  SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        env = ft_list_to_strs(sh->env_map);
        if (!env || !*env)
            exit(E_SYSTEM);
        execve(argv[0], argv, env);
        perror(argv[0]);
        exit(127);
    }
    toggle_sig_ignore(&sig_held);
    return (wait_and_status(n->args->pid));
}

/*******************************************************************************
 * ビルトイン & コマンド実行エントリ
 ******************************************************************************/

static int prepare_argv(t_ast *n, char ***out, t_shell *sh)
{
    struct stat sb;

    if (handle_redirections(n->args, sh))
        return (E_SYSTEM);
    *out = toklist_to_argv(n->args->argv, sh);
    if (!*out || !(*out)[0])
        return (E_NONE);
    if (stat((*out)[0], &sb) == 0 && S_ISDIR(sb.st_mode))
        return (E_IS_DIR);
    return (E_NONE);
}

int exe_cmd(t_ast *n, t_shell *sh)
{
    char        **argv;
    t_fdbackup  bk_in  = { -1, STDIN_FILENO };
    t_fdbackup  bk_out = { -1, STDOUT_FILENO };
    int         status;

    if (!n || n->ntype != NT_CMD)
        return (E_SYSTEM);
    if ((status = ms_lstiter(n->args->argv, proc_exec_path, sh)))
        return (status);
    if ((status = prepare_argv(n, &argv, sh)))
        return (status);

    /* ───── STDIN / STDOUT を差し替え ───── */
    if (n->args->fds[0] != -1)
    {
        fdbackup_enter(&bk_in, STDIN_FILENO, sh);
        xdup2(&n->args->fds[0], STDIN_FILENO, sh);
        close(n->args->fds[0]);
        n->args->fds[0] = -1;
    }
    if (n->args->fds[1] != -1)
    {
        fdbackup_enter(&bk_out, STDOUT_FILENO, sh);
        xdup2(&n->args->fds[1], STDOUT_FILENO, sh);
        close(n->args->fds[1]);
        n->args->fds[1] = -1;
    }

    /* ───── 実行 ───── */
    if (is_builtin(argv[0]))
        status = builtin_launch(argv, sh);
    else
        status = exec_external(argv, n, sh);

    /* ───── 復元 & 後片付け ───── */
    fdbackup_exit(&bk_in);
    fdbackup_exit(&bk_out);
    cleanup_redir_fds(n->args);
    return (status);
}

/*******************************************************************************
 * パイプ, AND/OR/LIST, サブシェル (run_pipe / run_boolean / run_subshell)
 *             ※ 本体ロジックは前版と変更なし
 ******************************************************************************/

static int run_pipe(t_ast *, t_shell *);
static int run_boolean(t_ast *, t_shell *);
static int run_subshell(t_ast *, t_shell *);

static int run_pipe(t_ast *n, t_shell *sh)
{
    int     fds[2];
    pid_t   lpid;
    pid_t   rpid;
    int     sig_held = 0;

    toggle_sig_ignore(&sig_held);
    xpipe(fds, sh);

    /* left */
    lpid = xfork(sh);
    if (lpid == 0)
    {
        xdup2(&fds[1], STDOUT_FILENO, sh);
        xclose(&fds[0]);
        xclose(&fds[1]);
        exit(exe_run(n->left, sh));
    }
    /* right */
    rpid = xfork(sh);
    if (rpid == 0)
    {
        xdup2(&fds[0], STDIN_FILENO, sh);
        xclose(&fds[0]);
        xclose(&fds[1]);
        exit(exe_run(n->right, sh));
    }
    xclose(&fds[0]);
    xclose(&fds[1]);
    wait_and_status(lpid);
    toggle_sig_ignore(&sig_held);
    return (wait_and_status(rpid));
}

static int run_boolean(t_ast *n, t_shell *sh)
{
    int st_left;

    if (sh->env_updated)
        mod_sem(sh, 0);
    sh->env_updated = 0;
    st_left = exe_run(n->left, sh);
    sh->status = st_left;
    xfree((void **)&sh->env_spc['?']);
    sh->env_spc['?'] = xitoa(st_left, sh);
    mod_sem(sh, 0);

    if ((n->ntype == NT_AND && st_left == 0) ||
        (n->ntype == NT_OR  && st_left != 0) ||
        (n->ntype == NT_LIST))
        return (exe_run(n->right, sh));
    return (st_left);
}

static int run_subshell(t_ast *n, t_shell *sh)
{
    pid_t pid = xfork(sh);

    if (pid == 0)
    {
        sh->interactive = 0;
        exit(exe_run(n->left, sh));
    }
    return (wait_and_status(pid));
}

/*******************************************************************************
 * exe_run ディスパッチ (他ユニットで定義)                                    		*
 ******************************************************************************/

int exe_run(t_ast *node, t_shell *sh);

int exe_pipe(t_ast *node, t_shell *sh) { return (run_pipe(node, sh)); }
int exe_bool(t_ast *node, t_shell *sh) { return (run_boolean(node, sh)); }
int exe_sub(t_ast *node, t_shell *sh)  { return (run_subshell(node, sh)); }


/* 旧 handle_redr/ heredo_into_fd は static 化して本ファイル内部専用に */
/* 他ファイルから共用する必要があれば、対応するヘッダを調整してください */


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
		return (1);
}

t_status	mod_exec(t_shell *sh)
{
	int	st;

	if (!sh || !sh->ast)
		return (E_NONE);
	st = exe_run(sh->ast, sh);
	sh->status = st;
	if (sh->debug & DEBUG_EXEC)
	{
		if (st != 0)
			printf("mod_exec Error: %d\n", st);
	}
	return (E_NONE);
}

char	**toklist_to_argv(t_list *lst, t_shell *sh)
{
	t_lexical_token	*tok;
	char			**argv;
	size_t			n;
	size_t			i;

	n = ft_lstsize(lst);
	argv = xmalloc_gcline(sizeof(char *) * (n + 1), sh);
	i = 0;
	while (lst)
	{
		tok = lst->data;
		argv[i++] = tok->value;
		lst = lst->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	heredoc_into_fd(char *body, t_args *args, t_shell *sh)
{
	int	hd[2];

	if (xpipe(hd, sh))
		return (1);
	if (write(hd[1], body, ft_strlen(body)) == -1 || close(hd[1]) == -1)
	{
		close(hd[0]);
		return (1);
	}
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	args->fds[0] = hd[0];
	return (0);
}
