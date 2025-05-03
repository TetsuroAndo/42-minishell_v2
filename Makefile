# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/22 01:37:23 by teando            #+#    #+#              #
#    Updated: 2025/05/03 14:16:35 by tomsato          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
OPT			:= -O2
RM			:= rm -rf
DEFINE		:= -DDEBUG_MODE=DEBUG_NONE

ROOT_DIR	:= .
SRC_DIR		:= $(ROOT_DIR)/src
INC_DIR		:= $(ROOT_DIR)/inc
OBJ_DIR		:= $(ROOT_DIR)/obj
LIBFT_DIR	:= $(ROOT_DIR)/src/lib/libft

IDFLAGS		:= -I$(INC_DIR) -I$(LIBFT_DIR)

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
lex: f
	$(MAKE) __lex -j $(shell nproc)
syn: f
	$(MAKE) __syn -j $(shell nproc)
sem: f	
	$(MAKE) __sem -j $(shell nproc)
debug: f
	$(MAKE) __debug -j $(shell nproc)

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBFT_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPT) $(IDFLAGS) $(DEFINE) -fPIC -MMD -MP -c $< -o $@

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

v: f $(NAME)

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

debug: OPT		:= -g -fsanitize=address -O1 -fno-omit-frame-pointer
debug: DEFINE	:= -DDEBUG_MODE=DEBUG_ALL
debug: f $(NAME)

# =======================
# == Submodule Targets ==
# =======================

norm:
	@norminette $(SRC) $(INC_DIR)

.PHONY: all clean fclean re norm
