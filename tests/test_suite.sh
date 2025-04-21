#!/usr/bin/env bash
# ================================================================
# 42 okyo minishell — lightweight regression test suite
# ---------------------------------------------------------------
#  ⚠️  42 Norm  & project requirements friendly: single file, POSIX‑sh
#      No external dependencies; works on the school VM out‑of‑the‑box.
#      Intended as a quick "smoke test" before pushing / evaluating.
#
#  HOW TO USE:
#    1.  Place this file at the repository root next to your minishell
#        executable (assumed to be ./minishell).
#    2.  chmod +x minishell_tests.sh
#    3.  ./minishell_tests.sh               # run all tests
#
#  A green “OK” ≈ identical behaviour to /bin/bash for the snippet.
#  A red  “KO”  means minishell diverged; inspect the diff in ./tests/out/.
#  You can add or comment tests by editing the TESTS array below.
# ================================================================

###############################################################################
# CONFIG 
###############################################################################
MINI="./minishell"        # path to your executable
BASH="/bin/bash"          # reference shell
OUT_DIR="tests/out"       # artefacts live here
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

###############################################################################
# INTERNAL HELPERS
###############################################################################
_colors() {
  if [ -t 1 ]; then            # colour only if stdout is a terminal
    _RED=$'\e[0;31m'; _GRN=$'\e[0;32m'; _YEL=$'\e[0;33m'; _RST=$'\e[0m'
  else
    _RED=''; _GRN=''; _YEL=''; _RST=''
  fi
}
_colors
_pass=0; _fail=0
_tmp() { mktemp "$OUT_DIR/$1.XXXX"; }

# Strip prompt noise then filter out:        
#   • any input lines we fed to minishell (readline echo)
#   • lone "exit" echoes that minishell prints in interactive mode
_clean() {
  local src="$1" pattern="$2" dst="$3"
  # drop prompt prefixes first (already done for bash/minishell names)
  # build grep pattern file on the fly
  local patfile=$(_tmp pat)
  printf '%s\n' exit >> "$patfile"        # always remove raw "exit"
  printf '%s\n' "$pattern" | while IFS= read -r l; do
    printf '%s\n' "$l" >> "$patfile"
  done
  # grep -vFxf : delete any full‑line match from patfile
  grep -vFxf "$patfile" "$src" > "$dst"
  rm -f "$patfile"
}

run_one() {
  local label="$1" code="$2"
  local ref_raw=$(_tmp ref) mini_raw=$(_tmp mini) diff_out=$(_tmp diff)
  local ref=$(_tmp refC) mini=$(_tmp miniC)

  # feed both shells (cmd + exit) so built‑ins affect same session
  printf '%s\n' "$code" exit | "$BASH" 2>&1 | \
    sed -e 's/^bash[[:digit:]: ]*[$#] //' > "$ref_raw"
  printf '%s\n' "$code" exit | "$MINI" 2>&1 | \
    sed -e 's/^minishell[[:digit:]: ]*[$#] //' > "$mini_raw"

  # sanitize echoes / extra exit lines
  _clean "$ref_raw"  "$code" "$ref"
  _clean "$mini_raw" "$code" "$mini"

  if diff -u "$ref" "$mini" > "$diff_out"; then
    printf "  %s%-30s%s : %sOK%s\n"  "$_YEL" "$label" "$_RST" "$_GRN" "$_RST"
    rm -f "$diff_out" "$ref" "$mini" "$ref_raw" "$mini_raw"
    ((_pass++))
  else
    printf "  %s%-30s%s : %sKO%s  (see %s)\n" "$_YEL" "$label" "$_RST" "$_RED" "$_RST" "$diff_out"
    ((_fail++))
  fi
}

################################################################################
# TEST CASES
################################################################################
# Format:  "label::command(s)" — Multiple commands separated by \n inside entry.
# Keep it short & explicit for easy diff reading.
read -r -d '' _TESTS <<'EOF'
# =========================== BUILTINS =================================
ECHO_SIMPLE::echo hello world
ECHO_QUOTES::echo "hello    world"
PWD::pwd
CD_PWD::cd /tmp && pwd
ENV_EXPORT::export FOO=42 && env | grep FOO
ENV_UNSET::export BAR=42 && unset BAR && env | grep BAR
EXIT_STATUS::false; echo $?
ECHO_N::echo -n no_newline && echo _second_line
ECHO_MULTI_N::echo -n -n -n chain && echo ok
CD_FAIL::cd /no/such/dir 2>/dev/null; echo EXIT:$?
PWD_AFTER_CD::cd /tmp && pwd | grep /tmp
EXIT_NUMERIC::exit 42
EXIT_NON_NUMERIC::exit foo 2>/dev/null || echo EXIT:$?
EXPORT_INVALID::export 1ABC=bad 2>/dev/null; echo EXIT:$?
UNSET_SPECIAL::export X=1 && unset X && env | grep X

# =========================== PIPES =====================================
PIPE_EXITCODE::false | true; echo $?
MULTI_PIPE_EXIT::echo a | tr a A | wc -c
PIPE_BUILTIN::echo foo | cat | wc -w
PIPE_SIMPLE::echo abc | tr a-z A-Z
MULTI_PIPE::echo 1 2 3 | cat | wc -w

# =========================== REDIRECTIONS =================================
REDIR_OUT::echo redir > /tmp/ms_test && cat /tmp/ms_test && rm /tmp/ms_test
REDIR_APPEND::echo first > /tmp/ms_app && echo second >> /tmp/ms_app && cat /tmp/ms_app && rm /tmp/ms_app
REDIR_IN::printf "input" > /tmp/ms_in && cat < /tmp/ms_in && rm /tmp/ms_in
REDIR_CHAIN::echo first > /tmp/ms_r && echo second >> /tmp/ms_r && cat /tmp/ms_r && rm /tmp/ms_r
REDIR_AMBIG::cat < /dev/null > /dev/null && echo ok
HEREDOC::cat <<EOF\nfoo\nbar\nEOF
HEREDOC_NO_EXPAND::export V=BYE && cat <<'EOF'\n$V\nEOF
HEREDOC_EXPAND::export V=HI && cat <<EOF\n$V\nEOF

# =========================== EXPANSION =====================================
VAR_EXPAND::export X=abc && echo $X
DOUBLE_QUOTE_EXPAND::export X=zzz && echo "!$X!"
SINGLE_QUOTE_LITERAL::export X=yyy && echo '!$X!'
WILDCARD::ls inc/*.h | head -n 1

# =========================== QUOTING ===================================
MIXED_QUOTES::echo "'"double_inside_single"'"

# ============================ BONUS (*) ================================
WILDCARD_BONUS::echo * ::BONUS
AND_OR_BONUS::false && echo no || echo yes ::BONUS

# =========================== ERROR HANDLING =================================
CMD_NOT_FOUND::nonexistent_cmd 2>/dev/null || echo error
SYNTAX_ERR::echo before | | echo after
PIPE_SYNTAX_ERR::echo before | | echo after 2>/dev/null || echo ERR:$?
REDIR_SYNTAX_ERR::cat < 2>/dev/null || echo ERR:$?
EOF
################################################################################

IFS=$'\n' read -r -d '' -a TESTS <<< "$_TESTS"
printf "\n%sMinishell quick test (%s cases)%s\n" "$_YEL" "${#TESTS[@]}" "$_RST"

for entry in "${TESTS[@]}"; do
  [[ -z "$entry" || "$entry" = \#* ]] && continue
  label="${entry%%::*}"
  cmd="${entry#*::}"
  run_one "$label" "$cmd"
done
printf "\nSummary: %s%d OK%s, %s%d KO%s\n" "$_GRN" "$_pass" "$_RST" "$_RED" "$_fail" "$_RST"
exit $_fail
