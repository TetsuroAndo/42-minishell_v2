# 実装方針

## 統合ポイント:

mod_syn 関数の後、mod_sem 関数の前にワイルドカード展開を実行する新しい関数 mod_wildcard を追加します。
または、ast_simple_cmd 関数内で引数を処理する際にワイルドカード展開を行います。

## メモリ管理:

すべてのメモリ割り当ては xmalloc を使用し、シェル構造体に関連付けます。
展開結果のリストは t_list 構造体を使用して管理します。

## エラーハンドリング:

メモリ割り当て失敗などのエラーが発生した場合は、適切なエラーメッセージを表示し、FAILURE を返します。
ディレクトリアクセス権限がない場合は、エラーメッセージを表示せずに展開を行わずに元のパターンを保持します。

## テスト計画:

- 基本的なワイルドカードパターン（\*, ?）のテスト
- 複合パターン（_.c, test?._）のテスト
- エスケープシーケンス（\*, \?）のテスト
- 存在しないパターンのテスト
- 大量のファイルがマッチする場合のパフォーマンステスト

## 入出力定義

### expand_wildcard

- 入力:
  - const char \*pattern: 展開するワイルドカードパターン
  - t_shell \*shell: シェル構造体
- 出力:
  - t_list \*: マッチしたファイル名のリスト（t_wildcard_match 構造体のリスト）
  - マッチするファイルがない場合は NULL

### expand_wildcards_in_ast

- 入力:
  - t_ast \*ast: 展開対象の AST
  - t_shell \*shell: シェル構造体
- 出力:
  - t_status: 成功時は SUCCESS、失敗時は FAILURE

### match_pattern

- 入力:
  - const char \*pattern: パターン文字列
  - const char \*filename: ファイル名
- 出力:
  - int: マッチする場合は 1、しない場合は 0

### sort_matches

- 入力:
  - t_list \*matches: マッチしたファイル名のリスト
- 出力:
  - t_list \*: ソートされたリスト

### scan_directory

- 入力:
  - const char \*dir_path: スキャンするディレクトリのパス
  - const char \*pattern: マッチングパターン
  - t_shell \*shell: シェル構造体
- 出力:
  - t_list \*: マッチしたファイル名のリスト
