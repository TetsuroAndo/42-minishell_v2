# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/22 01:37:23 by teando            #+#    #+#              #
#    Updated: 2025/04/21 16:47:33 by teando           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
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

# dev all
all: CFLAGS += -g -fsanitize=address -O1 -fno-omit-frame-pointer
all: DEFINE := -DDEBUG_MODE=DEBUG_ALL
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LFLAGS) $(IDFLAGS) $(DEFINE) -o $(NAME)
	@echo "====================="
	@echo "== Build Complete! =="
	@echo "====================="
	@echo "[Executable]: $(NAME)"
	@echo "[UNAME_S]: $(UNAME_S)"
	@echo "[LIBRARY]: $(LIBFT)"
	@echo "[INCLUDEDIR]: $(INC_DIR) $(LIBFT_DIR)"
	@echo "[Compiler flags/CFLAGS]: $(CFLAGS)"
	@echo "[Linker flags/LFLAGS]: $(LFLAGS)"
	@echo "[Debug flags/DEFINE]: $(DEFINE)"
	@echo "====================="

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBFT_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IDFLAGS) $(DEFINE) -fPIC -MMD -MP -c $< -o $@

$(LIBFT): | $(LIBFT_DIR)/libft.h
	$(MAKE) -C $(LIBFT_DIR)

c:
	$(RM) $(OBJ_DIR)
f: c
	$(RM) $(NAME)
r: f all

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# =======================
# == PRODUCTION =========
# =======================

v: CFLAGS += -O2
v: f $(NAME)

# =======================
# == DEBUG =============
# =======================

core: CFLAGS += -g -fsanitize=address -O1 -fno-omit-frame-pointer
core: DEFINE := -DDEBUG_MODE=DEBUG_CORE
core: f $(NAME)

debug: CFLAGS += -g -fsanitize=address -O1 -fno-omit-frame-pointer
debug: DEFINE := -DDEBUG_MODE=DEBUG_ALL
debug: f $(NAME)
	@echo "====================="
	@echo "== Build Complete! =="
	@echo "====================="
	@echo "[Executable]: $(NAME)"
	@echo "[UNAME_S]: $(UNAME_S)"
	@echo "[LIBRARY]: $(LIBFT)"
	@echo "[INCLUDEDIR]: $(INC_DIR) $(LIBFT_DIR)"
	@echo "[Compiler flags/CFLAGS]: $(CFLAGS)"
	@echo "[Linker flags/LFLAGS]: $(LFLAGS)"
	@echo "[Debug flags/DEFINE]: $(DEFINE)"
	@echo "====================="

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
