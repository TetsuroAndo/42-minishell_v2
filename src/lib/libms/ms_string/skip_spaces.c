
#include "libms.h"

/**
 * @brief 空白文字をスキップする
 *
 * @param line 処理対象の文字列
 * @param pos 現在の位置を示すポインタ
 */
void	skip_spaces(const char *line, size_t *pos)
{
	while (line[*pos] && ft_isspace(line[*pos]))
		(*pos)++;
}
