# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by student           #+#    #+#              #
#    Updated: 2025/09/14 14:18:15 by eahmeti          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
RM			= rm -rf

# Directories
SRCS_DIR	= srcs/
OBJS_DIR	= objs/
INC_DIR		= includes/
LIBFT_DIR	= srcs/libft/
MLX_DIR		= minilibx/

# Libraries
LIBFT		= $(LIBFT_DIR)libft.a
LIBMLX		= $(MLX_DIR)libmlx.a
MLX_FLAGS	= -framework OpenGL -framework AppKit  # MacOS
# MLX_FLAGS	= -lXext -lX11 -lm  # Linux

# Source files
SRCS		= main.c \
			  parsing/parser.c \
			  parsing/validate_map.c \
			  parsing/parser_utils.c \
			  graphics/init_mlx.c \
			  graphics/init_player.c \
			  graphics/event.c \
			  graphics/movement.c \
			  graphics/ray.c \
			  utils/errors.c \

# Objects
OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

# Colors for output
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RED			= \033[0;31m
NC			= \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(LIBMLX) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBMLX) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully$(NC)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR) -c $< -o $@
	@echo "$(YELLOW)○ Compiling: $(notdir $<)$(NC)"

$(LIBFT):
	@make -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)✓ libft compiled$(NC)"

$(LIBMLX):
	@make -C $(MLX_DIR) --no-print-directory
	@echo "$(GREEN)✓ miniLibX compiled$(NC)"

clean:
	@$(RM) $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@make -C $(MLX_DIR) clean --no-print-directory
	@echo "$(RED)✗ Objects cleaned$(NC)"

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@make -C $(MLX_DIR) clean --no-print-directory
	@echo "$(RED)✗ $(NAME) removed$(NC)"

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus