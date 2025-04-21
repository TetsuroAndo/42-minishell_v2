#include "libms.h"

// 2つのパスをcat(連結)しつつ重複した'/'を除去するユーティリティ関数
// dst: 出力バッファ, s: 入力パス, siz: バッファサイズ
// 戻り値: コピー後の長さ
size_t	ms_path_cleancat(char *dst, const char *s, size_t siz)
{
	size_t	didx;
	size_t	sidx;
	char	prev;

	sidx = 0;
	if (!dst || siz == 0)
		return (0);
	didx = ft_strlen(dst);
	prev = 0;
	if (s)
	{
		while (s[sidx] && didx < siz - 1)
		{
			if (s[sidx] == '/' && prev == '/')
			{
				sidx++;
				continue ;
			}
			dst[didx++] = s[sidx];
			prev = s[sidx++];
		}
	}
	dst[didx] = '\0';
	return (didx);
}
