# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/22 01:37:23 by teando            #+#    #+#              #
#    Updated: 2025/04/17 11:34:34 by teando           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CC			:= cc
CFLAGS		:= 
RM			:= rm -rf
DEFINE		:= -D DEBUG_ALL

# ディレクトリ設定
ROOT_DIR	:= .
SRC_DIR		:= $(ROOT_DIR)/src
INC_DIR		:= $(ROOT_DIR)/inc
OBJ_DIR		:= $(ROOT_DIR)/obj
LIBFT_DIR	:= $(ROOT_DIR)/src/lib/libft
LIBMS_DIR	:= $(ROOT_DIR)/src/lib/libms

# ライブラリ設定
LIBFT		:= $(LIBFT_DIR)/libft.a

# インクルードフラグ
IDFLAGS		:= -I$(INC_DIR) -I$(LIBFT_DIR)

# 環境依存
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LFLAGS		:= -lreadline -L$(shell brew --prefix readline)/lib
	IDFLAGS		+= -I$(shell brew --prefix readline)/include
endif
ifeq ($(UNAME_S),Linux)
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
OBJ		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# ビルドルール
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LFLAGS) $(IDFLAGS) $(DEFINE) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBFT_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IDFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

debug: CFLAGS += -g -fsanitize=address -O1 -fno-omit-frame-pointer
debug: re

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# =======================
# == Submodule Targets ==
# =======================

$(LIBFT_DIR):
	git submodule update --init --recursive

sub:
	git submodule update --remote

norm:
	@norminette $(SRCS) $(INC_DIR)

.PHONY: all clean fclean re norm fclean_libft fclean_libms
