#!/usr/bin/env bash
# extended_compare.sh
# Bash と minishell の動作を網羅的に比較するテストスクリプト
# ※セミコロンは使わず、AND/ORノードでコマンドを連結しています

MS=./minishell  # minishell 実行ファイルへのパス

# ANSI カラー設定
BOLD=$(tput bold)
RESET=$(tput sgr0)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
CYAN=$(tput setaf 6)
GREEN=$(tput setaf 2)

# テストケース: "説明|コマンド列"
tests=(
  "Built-in: echo -n|echo -n Hello && echo \$?"
  "Built-in: pwd (initial)|pwd"
  "Built-in: cd /tmp and back|cd /tmp && pwd && cd - >/dev/null && pwd"
  "Built-in: cd 不在ディレクトリ|cd /no_such || echo \$?"
  "Env export/unset|export TESTVAR=foo && echo \$TESTVAR && unset TESTVAR && echo \$TESTVAR"
  "特殊変数 \$\?|false || echo \$?"
  "変数展開: export A|export A=world && echo Hello \$A"
  "リダイレクト: 上書き|echo overw > f.txt && cat f.txt && rm f.txt"
  "リダイレクト: 追記|echo first > g.txt && echo second >> g.txt && cat g.txt && rm g.txt"
  "パイプライン|printf 'a\nb\n' | wc -l"
  "クオート: シングル|echo '\$HOME $(date)'"
  "クオート: ダブル|echo \"HOME=\$HOME literal=\$(date)\""
  "文字列中の \$\(\ )|echo literal\$\(\) test"
  "バックスラッシュ非解釈|echo foo\\bar"
)

run_case(){
  local desc="$1"
  local cmds="$2"
  echo
  echo "${BOLD}${YELLOW}=== ${desc} ===${RESET}"
  echo "${BLUE}-- Bash --${RESET}"
  bash -c "${cmds}; exit"
  echo "${CYAN}Exit status: $?${RESET}"
  echo
  echo "${BLUE}-- Minishell --${RESET}"
  printf '%s\nexit\n' "${cmds}" | "${MS}"
  ms_status=${PIPESTATUS[1]}
  echo "${CYAN}Exit status: ${ms_status}${RESET}"
}

for entry in "${tests[@]}"; do
  IFS="|" read -r desc cmds <<< "$entry"
  run_case "$desc" "$cmds"
done

echo
echo "${BOLD}${GREEN}=== 比較完了 ===${RESET}"
