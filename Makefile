NAME		= cub3D

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

MLX_DIR		= minilibx-linux
MLX_LIB		= $(MLX_DIR)/libmlx.a

INCLUDES	= -Iincludes -I$(MLX_DIR)

SRCS		= srcs/main.c \
			  srcs/parsing/parse_file.c \
			  srcs/parsing/parse_identifiers.c \
			  srcs/parsing/parse_colors.c \
			  srcs/parsing/parse_map.c \
			  srcs/parsing/validate_map.c \
			  srcs/parsing/validate_scene.c \
			  srcs/init/init_game.c \
			  srcs/init/init_player.c \
			  srcs/init/init_mlx.c \
			  srcs/init/load_textures.c \
			  srcs/engine/hooks.c \
			  srcs/engine/loop.c \
			  srcs/engine/movement.c \
			  srcs/engine/rotation.c \
			  srcs/engine/close.c \
			  srcs/render/render_frame.c \
			  srcs/render/render_background.c \
			  srcs/render/raycast.c \
			  srcs/render/draw_column.c \
			  srcs/utils/error.c \
			  srcs/utils/cleanup.c \
			  srcs/utils/free_scene.c \
			  srcs/utils/pixel.c \
			  srcs/utils/color.c \
			  srcs/utils/ft_string.c \
			  srcs/utils/ft_string2.c \
			  srcs/utils/get_next_line.c \
			  srcs/utils/get_next_line_utils.c \
			  srcs/parsing/parse_file_utils.c \
			  srcs/parsing/validate_map_utils.c \
			  srcs/render/raycast_utils.c \
			  srcs/init/init_player_utils.c 

OBJS		= $(SRCS:.c=.o)

LDFLAGS		= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

%.o: %.c includes/cub3d.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@if [ -f "$(MLX_DIR)/Makefile.gen" ]; then \
		$(MAKE) -C $(MLX_DIR) clean; \
	fi

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re