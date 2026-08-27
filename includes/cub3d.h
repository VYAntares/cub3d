#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "mlx.h"

# define SUCCESS 1
# define FAILURE 0

# define WINDOW_TITLE "cub3D"
# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720

# define MOVE_SPEED 0.01
# define ROT_SPEED 0.03

# define EVENT_KEY_PRESS 2
# define EVENT_KEY_RELEASE 3
# define EVENT_DESTROY 17

# define MASK_KEY_PRESS (1L << 0)
# define MASK_KEY_RELEASE (1L << 1)
# define MASK_DESTROY 0L

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef enum e_tex_id
{
	TEX_NO,
	TEX_SO,
	TEX_WE,
	TEX_EA
}	t_tex_id;

typedef enum e_side
{
	SIDE_X,
	SIDE_Y
}	t_side;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	value;
}	t_color;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_texture
{
	char	*path;
	t_img	img;
}	t_texture;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	char	spawn;
}	t_player;

typedef struct s_input
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_input;

typedef struct s_scene
{
	t_texture	textures[4];
	t_color		floor;
	t_color		ceiling;
	t_map		map;
	t_player	player;
}	t_scene;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	t_img	frame;
}	t_mlx;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_x;
}	t_ray;

typedef struct s_game
{
	t_scene		scene;
	t_mlx		mlx;
	t_input		input;
	int			screen_width;
	int			screen_height;
	double		move_speed;
	double		rot_speed;
}	t_game;

/*
** init
*/
void	init_game(t_game *game);
int		init_player_from_spawn(t_scene *scene);
int		init_mlx(t_game *game);
int		load_textures(t_game *game);

/*
** parsing / validation
*/
int		parse_file(const char *filename, t_scene *scene);
int		parse_texture_line(const char *line, t_scene *scene);
int		parse_color_line(const char *line, t_scene *scene);
int		parse_map_line(t_map *map, const char *line);
int		line_is_empty(const char *line);
int		line_is_map_start(const char *line);
int		validate_scene(t_scene *scene);
int		validate_map(t_scene *scene);

/*
** engine
*/
int		init_hooks(t_game *game);
int		game_loop(t_game *game);
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
void	move_player(t_game *game);
void	rotate_left(t_game *game);
void	rotate_right(t_game *game);
int		close_game(t_game *game);

/*
** render
*/
void	render_frame(t_game *game);
void	render_background(t_game *game);
void	raycast_frame(t_game *game);
void	draw_column(t_game *game, t_ray *ray, int x);

/*
** utils
*/
int		error_msg(const char *msg);
int		fail_game(t_game *game, const char *msg);
void	free_scene(t_scene *scene);
void	cleanup_game(t_game *game);
void	put_pixel(t_img *img, int x, int y, int color);
int		rgb_to_int(int r, int g, int b);
int		is_valid_rgb_value(int value);

/*
** local string / gnl
*/
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		ft_isdigit(int c);
char	*get_next_line(int fd);

#endif