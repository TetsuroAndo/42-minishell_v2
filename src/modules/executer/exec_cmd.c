/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:43:40 by teando            #+#    #+#             */
/*   Updated: 2025/05/03 15:54:37 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

static void	fdbackup_restore(t_fdbackup *bk, t_shell *sh)
{
	if (bk->saved != -1)
		xdup2(&bk->saved, bk->target, sh);
}

static int	prepare_argv(t_ast *node, char ***out, int *flag, t_shell *sh)
{
	struct stat	sb;

	if (handle_redr(node->args, sh))
	{
		*flag = 1;
		return (E_SYSTEM);
	}
	*out = toklist_to_argv(node->args->argv, sh);
	if (!*out)
	{
		*flag = 1;
		return (E_SYSTEM);
	}
	if (!(*out)[0])
	{
		*flag = 1;
		return (E_NONE);
	}
	if (stat((*out)[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		return (E_IS_DIR);
	return (E_NONE);
}

static void	setup_redirections(t_ast *node, t_fdbackup *bk_in,
		t_fdbackup *bk_out, t_shell *sh)
{
	*bk_in = (t_fdbackup){-1, STDIN_FILENO};
	*bk_out = (t_fdbackup){-1, STDOUT_FILENO};
	if (node->args->fds[0] != -1)
	{
		fdbackup_enter(bk_in, STDIN_FILENO, sh);
		xdup2(&node->args->fds[0], STDIN_FILENO, sh);
	}
	if (node->args->fds[1] != -1)
	{
		fdbackup_enter(bk_out, STDOUT_FILENO, sh);
		xdup2(&node->args->fds[1], STDOUT_FILENO, sh);
	}
}

static int	execv_external(char **argv, t_ast *node, t_shell *sh)
{
	int		sig_held;
	char	**env;

	sig_held = 0;
	sig_ignore_parent(&sig_held);
	node->args->pid = xfork(sh);
	if (node->args->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		env = ft_list_to_strs(sh->env_map);
		if (!*env)
			return (ft_strs_clear(env), E_SYSTEM);
		if (!(*env)[0])
			return (ft_strs_clear(env), E_NONE);
		execve(argv[0], argv, env);
		perror(argv[0]);
		exit(127);
	}
	sig_ignore_parent(&sig_held);
	return (wait_and_status(node->args->pid));
}

int	exe_cmd(t_ast *node, t_shell *sh)
{
	char		**argv;
	t_fdbackup	bk_in;
	t_fdbackup	bk_out;
	int			status;
	int			flag;

	flag = 0;
	argv = NULL;
	if (!node || node->ntype != NT_CMD)
		return (1);
	status = ms_lstiter(node->args->argv, proc_exec_path, sh);
	if (status)
		return (status);
	status = prepare_argv(node, &argv, &flag, sh);
	if (flag)
		return (status);
	setup_redirections(node, &bk_in, &bk_out, sh);
	if (is_builtin(argv[0]))
		status = builtin_launch(argv, sh);
	else
		status = execv_external(argv, node, sh);
	fdbackup_restore(&bk_in, sh);
	fdbackup_restore(&bk_out, sh);
	cleanup_redir_fds(node->args);
	return (status);
}
