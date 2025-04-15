# シェルにおけるクォート処理の詳細

シェルにおけるクォート処理は、コマンドライン引数の解釈方法を制御する重要な機能です。42 minishell プロジェクトでのクォート処理について詳細に説明します。

## クォートの種類と基本動作

### 1. シングルクォート（'）

- **最も強力なクォート**: 内部のすべての文字をリテラルとして扱います
- **特徴**:
  - 変数展開（`$VAR`）は行われません
  - ワイルドカード展開（`*`, `?`）は行われません
  - エスケープ文字（`\`）も特殊な意味を持ちません
  - シングルクォート自体はエスケープできません
- **例**:
  ```bash
  echo '$HOME'  # 出力: $HOME (展開されない)
  echo '*.txt'  # 出力: *.txt (ワイルドカード展開されない)
  echo 'Hello\'World'  # シンタックスエラー (シングルクォート内のバックスラッシュはエスケープとして機能しない)
  ```

### 2. ダブルクォート（"）

- **中程度のクォート**: 一部の特殊文字の機能を保持します
- **特徴**:
  - 変数展開（`$VAR`）は行われます
  - ワイルドカード展開（`*`, `?`）は行われません
  - バックスラッシュ（`\`）は特定の文字（`$`, `"`, `\`, `` ` ``）の前でのみエスケープとして機能します
- **例**:
  ```bash
  echo "$HOME"  # 出力: /home/user (変数が展開される)
  echo "*.txt"  # 出力: *.txt (ワイルドカード展開されない)
  echo "Hello\"World"  # 出力: Hello"World (ダブルクォートがエスケープされる)
  ```

### 3. バックスラッシュ（\）

- **単一文字のエスケープ**: 次の 1 文字の特殊な意味を無効化します
- **特徴**:
  - 直後の 1 文字のみに影響します
  - 改行をエスケープすると、コマンドが次の行に続きます
- **例**:
  ```bash
  echo \$HOME  # 出力: $HOME (変数展開されない)
  echo \*.txt  # 出力: *.txt (ワイルドカード展開されない)
  ```

## クォート処理の実装詳細

### 1. クォートの状態管理

クォート処理を実装する際は、パーサーが現在どのクォート状態にあるかを追跡する必要があります：

```c
typedef enum e_quote_state {
    QUOTE_NONE,       // クォートなし
    QUOTE_SINGLE,     // シングルクォート内
    QUOTE_DOUBLE      // ダブルクォート内
    BACK_SLUSH
} t_quote_state;
```

### 2. クォート除去と内容処理

トークン内のクォートを処理する際の基本的なアルゴリズム：

1. 入力文字列を 1 文字ずつ走査
2. クォート文字（`'` または `"`）に遭遇したら状態を切り替え
3. クォート内の文字を適切に処理（展開するかしないか）
4. 最終的な文字列からクォート自体を除去

```c
/*
** クォートを処理して新しい文字列を返す関数
** str: 処理する文字列
** shell: シェル構造体
** returns: クォート処理された新しい文字列
*/
char *process_quotes(const char *str, t_shell *shell)
{
    t_quote_state state = QUOTE_NONE;
    char *result = NULL;
    // 実装...
    return result;
}
```

### 3. 変数展開とクォート

変数展開とクォートの相互作用を処理する必要があります：

```c
/*
** クォート状態に応じて変数展開を行うかどうかを決定する関数
** str: 処理する文字列
** pos: 現在の位置（$の位置）
** state: 現在のクォート状態
** shell: シェル構造体
** returns: 展開された文字列
*/
char *expand_variable_with_quotes(const char *str, int pos, t_quote_state state, t_shell *shell)
{
    // シングルクォート内なら展開しない
    if (state == QUOTE_SINGLE)
        return ft_strdup("$");

    // それ以外なら展開する
    return expand_variable(str, pos, shell);
}
```

### 4. ワイルドカード展開とクォート

ワイルドカード展開もクォート状態に依存します：

```c
/*
** クォート状態に応じてワイルドカード展開を行うかどうかを決定する関数
** pattern: 展開するパターン
** state: クォート状態
** shell: シェル構造体
** returns: 展開されたファイル名のリスト
*/
t_list *expand_wildcard_with_quotes(const char *pattern, t_quote_state state, t_shell *shell)
{
    // クォート内ならワイルドカード展開しない
    if (state == QUOTE_SINGLE || state == QUOTE_DOUBLE)
        return NULL;

    // クォート外ならワイルドカード展開する
    return expand_wildcard(pattern, shell);
}
```

### 5. クォートの不一致エラー処理

クォートが閉じられていない場合のエラー処理も重要です：

```c
/*
** クォートが正しく閉じられているか確認する関数
** str: チェックする文字列
** returns: 正しく閉じられていれば1、そうでなければ0
*/
int check_quote_balance(const char *str)
{
    t_quote_state state = QUOTE_NONE;
    int i = 0;

    while (str[i])
    {
        if (str[i] == '\'' && state == QUOTE_NONE)
            state = QUOTE_SINGLE;
        else if (str[i] == '\'' && state == QUOTE_SINGLE)
            state = QUOTE_NONE;
        else if (str[i] == '\"' && state == QUOTE_NONE)
            state = QUOTE_DOUBLE;
        else if (str[i] == '\"' && state == QUOTE_DOUBLE)
            state = QUOTE_NONE;
        i++;
    }

    return (state == QUOTE_NONE);
}
```

## クォート処理の統合ポイント

minishell プロジェクトでは、クォート処理は主に以下の場所で行われます：

1. **字句解析（lexical analysis）段階**:

   - クォートを含む文字列をトークン化する際に、クォートの開始と終了を追跡
   - クォートが閉じられていない場合はエラー処理

2. **意味解析（semantic analysis）段階**:

   - トークン内のクォートに基づいて、変数展開やワイルドカード展開の適用を決定
   - クォート自体を最終的な引数から除去

3. **コマンド実行前の最終処理**:
   - クォート処理された引数を最終的なコマンド引数配列（argv）に変換

## 実装上の注意点

1. **ネストされたクォート**:

   ```bash
   echo "This is a 'quoted' string"
   ```

   外側のダブルクォートが処理されると、内側のシングルクォートは通常の文字として扱われます。

2. **エスケープされたクォート**:

   ```bash
   echo "This is a \"quoted\" string"
   ```

   ダブルクォート内でエスケープされたダブルクォートは、クォートの開始/終了ではなく文字として扱われます。

3. **複数行にまたがるクォート**:

   ```bash
   echo "This is a
   multiline string"
   ```

   クォートは複数行にまたがることができます（readline の動作に依存）。

4. **空のクォート**:
   ```bash
   echo ""  # 空文字列
   echo ''  # 空文字列
   ```
   空のクォートは空文字列として扱われます。

これらの詳細を考慮してクォート処理を実装することで、シェルの標準的な動作に準拠した minishell を作成できます。
