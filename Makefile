# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/22 01:37:23 by teando            #+#    #+#              #
#    Updated: 2025/04/28 20:59:28 by teando           ###   ########.fr        #
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
		minishell.c \
	)
SRC		+= $(shell find $(SRC_DIR)/core -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/lib/libms -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/modules/analyze_lexical -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/modules/analyze_syntax -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/modules/analyze_semantic -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/modules/executer -name '*.c')
SRC		+= $(shell find $(SRC_DIR)/builtin_cmds -name '*.c')
OBJ		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Index
all:
	$(MAKE) __build -j $(shell nproc)
v: f
	$(MAKE) __v -j $(shell nproc)
core: f
	$(MAKE) __core -j $(shell nproc)
debug: f
	$(MAKE) __debug -j $(shell nproc)

__build: OPT	:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__build: DEFINE	:= -DDEBUG_MODE=DEBUG_ALL
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

__debug: OPT 	:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
__debug: DEFINE	:= -DDEBUG_MODE=DEBUG_ALL
__debug: $(NAME)

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
