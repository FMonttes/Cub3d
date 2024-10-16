/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:40:20 by felperei          #+#    #+#             */
/*   Updated: 2024/10/16 14:24:09 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H
#include "./libft/libft.h"
#include "../../../minilibx/mlx.h"
#include "../../../minilibx/mlx_int.h"
#include <X11/X.h>
#include <math.h>
#include <X11/keysym.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdlib.h>
#define M_PI 3.14159265358979323846
#define ESC 65307
#define KEY_A 97
#define KEY_W 119
#define KEY_D 100
#define KEY_S 115
#define KEY_Q 113
#define KEY_LEFT 65361
#define KEY_UP 65362
#define KEY_RIGHT 65363
#define KEY_DOWN 65364
#define MLX_ERROR 1
#define S_W 1920           // screen width
#define S_H 1015           // screen height
#define TILE_SIZE 64       // tile size
#define FOV 60             // field of view
#define ROTATION_SPEED 0.6 // rotation speed
#define PLAYER_SPEED 4     // player speed
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
typedef struct s_player {
    double plyr_x;
    double plyr_y;
    double fov_rd;
    double angle;
} t_player;

typedef struct s_ray {
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} t_ray;

typedef struct s_textures
{
    void *north;
    int north_w;
    int north_h;
    void *south;
    int south_w;
    int south_h;
    void *east;
    int east_w;
    int east_h;
    void *west;
    int west_w;
    int west_h;
    
} t_textures;


typedef struct s_data // the data structure
{
    char **backup;
    char **map2d; // the map
    int **map; // the map
    int p_x;      // player x position in the map
    int p_y;      // player y position in the map
    int w_map;    // map width
    int h_map;
    char *texData;
    char *imgData;
    double p_dir_x;
    double p_dir_y;
    double plane_x;
    double plane_y;
    int rows;
    int cols;
    char **textu;
    int buffer[S_W * S_H];
    char **c_flor; // map height
} t_data;

typedef struct s_raycast {
    double rayDirX;
    double rayDirY;
    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;
    int stepX;
    int stepY;
    int hit;
    int side;
} t_raycast;

typedef struct s_mlx {
    void *mlx_p;
    void *win;
   void *img_ptr;
    t_player *ply;
    t_ray *ray;
    t_textures *textures;
    t_data *dt;
    t_raycast *rc; // Raycasting data
} t_mlx;

// void update_map(t_mlx *mlx);
// int mlx_key(int keycode, void *ml);
// void ft_reles(int keycode, t_mlx *mlx);
// int ft_exit(t_mlx *mlx); // exit the game
// void cast_rays(t_mlx *mlx);
// void render_wall(t_mlx *mlx, int ray);
// void hook(t_mlx *mlx, double move_x, double move_y);
// float nor_angle(float angle);
// void load_textures(t_mlx *mlx);
// void	move_left(t_mlx *mlx);
// void move_player_backward(t_mlx *mlx);
// void game_events(int keycode, t_mlx *game);
// char **read_map(char *path);
// int keypress(int keycode, t_mlx *game);
// void	size_map(t_data *dt);
// void init_the_player(t_mlx *mlx);
// void draw_buffer(void *mlx_ptr, void *win_ptr, void *img_ptr, int *buffer);
// int get_texture_color(void *img_ptr, int x, int y);
// void calculate_step_and_side_dist(t_raycast *rc, double posX, double posY, int mapX, int mapY);
// void perform_dda(int *mapX, int *mapY, t_raycast *rc, t_mlx *mlx);
// double calculate_perp_wall_dist(int side, double posX, double posY, int mapX, int mapY, t_raycast *rc);
// void calculate_line_height(int *lineHeight, int *drawStart, int *drawEnd, double perpWallDist);
// void render_column(int x, int drawStart, int drawEnd, int texX, void *tex_ptr, int *buffer);
// void raycasting(t_mlx *mlx, int *buffer);
// void start_the_game(t_data *dt);

// MAP
int ft_exit(t_mlx *mlx);
char **read_map(char *path);
char **get_map(char **path);
int **copy_map(char **map, t_data *dt);
void size_map(t_data *dt);
void update_map(t_mlx *mlx);
int **copy_char_to_int(char **map, t_data *dt);

//PLAYER

void move_player_forward(t_mlx *mlx);
void move_player_backward(t_mlx *mlx);
void rotate_player(t_mlx *mlx, int i);
int keypress(int keycode, t_mlx *game);
void game_events(int keycode, t_mlx *game);

//RAYCASTING

void draw_buffer(void *mlx_ptr, void *win_ptr, void *img_ptr, int *buffer);
int get_texture_color(void *img_ptr, int x, int y);
void calculate_step_and_side_dist(t_raycast *rc, double posX, double posY, int mapX, int mapY);
void perform_dda(int *mapX, int *mapY, t_raycast *rc, t_mlx *mlx);
double calculate_perp_wall_dist(int side, double posX, double posY, int mapX, int mapY, t_raycast *rc);
void calculate_line_height(int *lineHeight, int *drawStart, int *drawEnd, double perpWallDist);
void render_column(int x, int drawStart, int drawEnd, int texX, void *tex_ptr, int *buffer);
void raycasting(t_mlx *mlx, int *buffer);

#endif