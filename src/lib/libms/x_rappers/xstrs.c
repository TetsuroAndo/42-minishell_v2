
#include "core.h"

/**
 * @brief 失敗時に終了する安全な連結リストノード作成ラッパー関数
 *
 * この関数は以下の処理を行います：
 * 1. 提供されたデータで新しいリストノードを作成
 * 2. ノード作成に失敗した場合はプログラムを終了
 *
 * @param data 新しいノードに格納する文字列データ
 * @param shell エラー処理用のシェルメイン情報構造体
 * @return t_list* 新しいリストノードへのポインタ
 */
t_list	*xlstnew(char *data, t_shell *shell)
{
	t_list	*list;

	list = ft_lstnew((void *)data);
	if (!list)
		shell_exit(shell, E_ALLOCATE);
	return (list);
}

// t_list	*xlst_from_strs(char **strs, t_shell *shell)
// {
// 	t_list	*list;

// 	list = ft_list_from_strs(strs);
// 	if (!list)
// 		shell_exit(shell, E_ALLOCATE);
// 	return (list);
// }

// char	**xlst_to_strs(t_list *lst, t_shell *shell)
// {
// 	char	**strs;

// 	strs = ft_list_to_strs(lst);
// 	if (!strs)
// 		shell_exit(shell, E_ALLOCATE);
// 	return (strs);
// }

// char	**xsplit(char *str, char sep, t_shell *shell)
// {
// 	char	**split;

// 	split = ft_split(str, sep);
// 	if (!split)
// 		shell_exit(shell, E_ALLOCATE);
// 	return (split);
// }
