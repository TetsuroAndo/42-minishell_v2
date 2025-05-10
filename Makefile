# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/22 01:37:23 by teando            #+#    #+#              #
#    Updated: 2025/05/10 22:28:47 by teando           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
OPT			:= -O2
RM			:= rm -rf
DEFINE		:= -DDEBUG_MODE=DEBUG_NONE

# ディレクトリ設定
ROOT_DIR	:= .
SRC_DIR		:= $(ROOT_DIR)/src
INC_DIR		:= $(ROOT_DIR)/inc
OBJ_DIR		:= $(ROOT_DIR)/obj
LIBFT_DIR	:= $(ROOT_DIR)/src/lib/libft

# インクルードフラグ
IDFLAGS		:= -I$(INC_DIR) -I$(LIBFT_DIR)

# 環境依存
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIBFT		:= $(LIBFT_DIR)/libft_mac.a
	LFLAGS		:= -lreadline -L$(shell brew --prefix readline)/lib
	IDFLAGS		+= -I$(shell brew --prefix readline)/include
else
	LIBFT		:= $(LIBFT_DIR)/libft.a
	LFLAGS		:= -lreadline
	IDFLAGS		+= -I/usr/include/readline
endif

# source files
SRC	:= \
	$(addprefix $(SRC_DIR)/, \
		core/shell_init.c \
		core/finalize.c \
		core/readline.c \
		core/line_init.c \
		core/init_signals.c \
		core/debug/init_sh_proc.c \
		core/debug/line_init_info.c \
		core/debug/fin_sh.c \
		builtin_cmds/exit.c \
		builtin_cmds/true.c \
		builtin_cmds/unset.c \
		builtin_cmds/env.c \
		builtin_cmds/pwd.c \
		builtin_cmds/export.c \
		builtin_cmds/cd.c \
		builtin_cmds/builtin_launcher.c \
		builtin_cmds/echo.c \
		lib/libms/ms_env/ms_unset.c \
		lib/libms/ms_env/ms_envcmp.c \
		lib/libms/ms_env/value/ms_escapevalue.c \
		lib/libms/ms_env/value/ms_isactivevalue.c \
		lib/libms/ms_env/key/ms_isactiveky.c \
		lib/libms/ms_env/key/ms_partenvarg.c \
		lib/libms/ms_env/ms_getenv.c \
		lib/libms/ms_env/ms_setenv.c \
		lib/libms/ms_ast/free_ast.c \
		lib/libms/ms_lexical/free_token.c \
		lib/libms/ms_gc/ms_strjoin_gc.c \
		lib/libms/ms_gc/ms_gc_track.c \
		lib/libms/ms_gc/ms_substr_gc.c \
		lib/libms/ms_gc/ms_strdup_gc.c \
		lib/libms/ms_gc/xfree_gc.c \
		lib/libms/ms_gc/xmalloc_gc.c \
		lib/libms/ms_gc/xlstnew_gc.c \
		lib/libms/ms_lst/ms_lstcopy.c \
		lib/libms/ms_lst/xlstnew.c \
		lib/libms/ms_lst/xlst_from_strs.c \
		lib/libms/ms_lst/ms_listshift.c \
		lib/libms/ms_lst/ms_lstiter.c \
		lib/libms/ms_lst/xlst_to_strs.c \
		lib/libms/ms_fd/xdup.c \
		lib/libms/ms_fd/set_cloexec.c \
		lib/libms/ms_fd/ms_set_cloexec.c \
		lib/libms/ms_fd/xclose.c \
		lib/libms/ms_path/path_cleancat.c \
		lib/libms/ms_path/path_relative.c \
		lib/libms/ms_path/path_cleanstrlcpy.c \
		lib/libms/ms_path/path_resolve.c \
		lib/libms/ms_path/is_builtin.c \
		lib/libms/ms_path/path_launcher.c \
		lib/libms/ms_path/path_home.c \
		lib/libms/ms_string/xsplit.c \
		lib/libms/ms_string/skip_quote.c \
		lib/libms/ms_string/is_quote.c \
		lib/libms/ms_string/skip_dollar_paren.c \
		lib/libms/ms_string/ms_strdup.c \
		lib/libms/ms_string/xstrjoin.c \
		lib/libms/ms_string/xitoa.c \
		lib/libms/ms_string/ms_put_ascii.c \
		lib/libms/ms_string/ms_substr.c \
		lib/libms/ms_string/skip_spaces.c \
		lib/libms/ms_string/trim_valid_quotes.c \
		lib/libms/ms_system/xfree.c \
		lib/libms/ms_system/xpipe.c \
		lib/libms/ms_system/xfork.c \
		lib/libms/ms_system/xmalloc.c \
		modules/analyze_lexical/lexer_main_loop.c \
		modules/analyze_lexical/allocate_token.c \
		modules/analyze_lexical/debug_putlex.c \
		modules/analyze_lexical/heredoc_loop.c \
		modules/analyze_lexical/lexer_handler.c \
		modules/analyze_lexical/tokenizer.c \
		modules/analyze_syntax/parse_andor.c \
		modules/analyze_syntax/parse_primary.c \
		modules/analyze_syntax/parse_pipeline.c \
		modules/analyze_syntax/parse_alloc.c \
		modules/analyze_syntax/debug_putsyn.c \
		modules/analyze_syntax/parse_utils.c \
		modules/analyze_syntax/parser.c \
		modules/analyze_semantic/split/del_nul_node.c \
		modules/analyze_semantic/split/split_utils.c \
		modules/analyze_semantic/split/split_with_quote.c \
		modules/analyze_semantic/quote/proc_quote.c \
		modules/analyze_semantic/path/res_exec_path.c \
		modules/analyze_semantic/semantic.c \
		modules/analyze_semantic/env_expand/quote_helper.c \
		modules/analyze_semantic/env_expand/proc_env.c \
		modules/analyze_semantic/wildcard/wc_parser_utils.c \
		modules/analyze_semantic/wildcard/wildcard_utils.c \
		modules/analyze_semantic/wildcard/wildcard_utils_2.c \
		modules/analyze_semantic/wildcard/wc_quote_parser.c \
		modules/analyze_semantic/wildcard/wildcard.c \
		modules/analyze_semantic/proc_redr_errs.c \
		modules/analyze_semantic/backup/astlst_backup.c \
		modules/analyze_semantic/debug/debug_putsem.c \
		modules/executer/executer.c \
		modules/executer/exec_bool.c \
		modules/executer/exec_cmd.c \
		modules/executer/exec_subsh.c \
		modules/executer/exec_pipe.c \
		modules/executer/exec_utils.c \
		modules/executer/exec_redirect.c \
		minishell.c \
	)
OBJ		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Index
all:
	$(MAKE) __build -j $(shell nproc)
v: f
	$(MAKE) __v -j $(shell nproc)
core: f
	$(MAKE) __core -j $(shell nproc)
lex: f
	$(MAKE) __lex -j $(shell nproc)
syn: f
	$(MAKE) __syn -j $(shell nproc)
sem: f	
	$(MAKE) __sem -j $(shell nproc)
debug: f
	$(MAKE) __debug -j $(shell nproc)
none_prompt: f
	$(MAKE) __none_prompt -j $(shell nproc)

__build: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OPT) $(OBJ) $(LIBFT) $(LFLAGS) $(IDFLAGS) $(DEFINE) -o $(NAME)
	@echo "====================="
	@echo "== Build Complete! =="
	@echo "====================="
	@echo "[Executable]: $(NAME)"
	@echo "[UNAME_S]: $(UNAME_S)"
	@echo "[Library]: $(LIBFT)"
	@echo "[Includedir]: $(INC_DIR) $(LIBFT_DIR)"
	@echo "[Compiler flags/CFLAGS]: $(CFLAGS)"
	@echo "[Optimizer flags/OPT]: $(OPT)"
	@echo "[Linker flags/LFLAGS]: $(LFLAGS)"
	@echo "[Debug flags/DEFINE]: $(DEFINE)"
	@echo "====================="

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPT) $(IDFLAGS) $(DEFINE) -fPIC -MMD -MP -c $< -o $@

$(LIBFT): | $(LIBFT_DIR)/libft.h
	$(MAKE) -C $(LIBFT_DIR)

c:
	$(RM) $(OBJ_DIR)
f: c
	$(RM) $(NAME)
r:
	$(MAKE) f
	$(MAKE) __build -j $(shell nproc)

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re:
	$(MAKE) fclean
	$(MAKE) __build -j $(shell nproc)

# =======================
# == PRODUCTION =========
# =======================

__v: $(NAME)

# =======================
# == DEBUG =============
# =======================

__core: OPT		:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__core: DEFINE	:= -DDEBUG_MODE=DEBUG_CORE
__core: $(NAME)

__lex: OPT		:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__lex: DEFINE	:= -DDEBUG_MODE=DEBUG_LEX
__lex: $(NAME)

__syn: OPT		:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__syn: DEFINE	:= -DDEBUG_MODE=DEBUG_SYN
__syn: $(NAME)

__sem: OPT		:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__sem: DEFINE	:= -DDEBUG_MODE=DEBUG_SEM
__sem: $(NAME)

__debug: OPT 	:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__debug: DEFINE	:= -DDEBUG_MODE=DEBUG_ALL
__debug: $(NAME)

__none_prompt: DEFINE	:= -DDEBUG_MODE=DEBUG_NO_PROMPT
__none_prompt: $(NAME)

# =======================
# == Submodule Targets ==
# =======================

$(LIBFT_DIR)/libft.h:
	git submodule update --remote --init --recursive

sub:
	git submodule update --remote

norm:
	@norminette $(SRC) $(INC_DIR)

.PHONY: all clean fclean re norm
