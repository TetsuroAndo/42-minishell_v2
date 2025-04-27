#!/usr/bin/env bash
# tests.sh

MS=./minishell  # minishell の実行ファイルパス

run() {
  echo "---- $1 ----"
  # minishell にコマンドを１行流して exit。標準出力と終了ステータスを表示
  OUTPUT=$($MS <<EOF
$2
exit
EOF
)
  STATUS=$?
  echo "Command: $2"
  echo "Output: $OUTPUT"
  echo "Status: $STATUS"
  echo
}

# 1) 括弧なし：&& は || より優先
run "No paren: false || echo A && echo B" \
    "false || echo A && echo B"
# Bash と同様、echo A と echo B の両方が実行されるはず
# → Output: A\nB, Status: 0

# 2) 括弧あり：先に || を評価
run "With paren: (false || echo A) && echo B" \
    "(false || echo A) && echo B"
# (false || echo A) がまず A を出力して成功→B も実行
# → Output: A\nB, Status: 0

# 3) 括弧あり：先に && を評価（意味は変わらない例）
run "With paren: false && (echo A || echo B)" \
    "false && (echo A || echo B)"
# false なので括弧内は実行されず、何も出力されない
# → Output: (空行), Status: 1

# 4) ネストした括弧
run "Nested: ( true || (echo X && false) ) && echo OK" \
    "( true || (echo X && false) ) && echo OK"
# 内部 (echo X && false) は X を出力して失敗→外側 true || … は成功→OK
# → Output: X\nOK, Status: 0

# 5) 複合：括弧で評価順を変えてみる
run "Change order: true || echo A && echo B" \
    "true || echo A && echo B"
# デフォルト：&& が先→ (true || (echo A && echo B)) → true なので何も実行
# → Output: (空行), Status: 0

run "Forced order: (true || echo A) && echo B" \
    "(true || echo A) && echo B"
# (true || echo A) → true → && echo B で B を実行
# → Output: B, Status: 0
