#include "../cub3d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};


void draw_buffer(void *mlx_ptr, void *win_ptr, void *img_ptr, int *buffer) {
    int x, y;
    char *data = mlx_get_data_addr(img_ptr, &x, &x, &x);
    for (y = 0; y < S_H; y++) {
        for (x = 0; x < S_W; x++) {
            int color = buffer[y * S_W + x];
            data[(y * S_W + x) * 4] = color & 0xFF;
            data[(y * S_W + x) * 4 + 1] = (color >> 8) & 0xFF;
            data[(y * S_W + x) * 4 + 2] = (color >> 16) & 0xFF;
        }
    }
    mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, 0, 0);
}

int get_texture_color(void *img_ptr, int x, int y) {
    char *data;
    int bpp, size_line, endian;
    data = mlx_get_data_addr(img_ptr, &bpp, &size_line, &endian);
    return *(int *)(data + (y * size_line + x * (bpp / 8)));
}

void calculate_step_and_side_dist(t_raycast *rc, double posX, double posY, int mapX, int mapY) {
    if (rc->rayDirX < 0) {
        rc->stepX = -1;
        rc->sideDistX = (posX - mapX) * rc->deltaDistX;
    } else {
        rc->stepX = 1;
        rc->sideDistX = (mapX + 1.0 - posX) * rc->deltaDistX;
    }
    if (rc->rayDirY < 0) {
        rc->stepY = -1;
        rc->sideDistY = (posY - mapY) * rc->deltaDistY;
    } else {
        rc->stepY = 1;
        rc->sideDistY = (mapY + 1.0 - posY) * rc->deltaDistY;
    }
}

void perform_dda(int *mapX, int *mapY, t_raycast *rc, t_mlx*mlx) {
	(void)mlx;
    while (!(rc->hit)) {
        if (rc->sideDistX < rc->sideDistY) {
            rc->sideDistX += rc->deltaDistX;
            *mapX += rc->stepX;
            rc->side = 0;
        } else {
            rc->sideDistY += rc->deltaDistY;
            *mapY += rc->stepY;
            rc->side = 1;
        }
        if (mlx->dt->map[*mapX][*mapY] > 0)
            rc->hit = 1;
    }
}

double calculate_perp_wall_dist(int side, double posX, double posY, int mapX, int mapY, t_raycast *rc) {
    if (side == 0)
        return (mapX - posX + (1 - rc->stepX) / 2) / rc->rayDirX;
    else
        return (mapY - posY + (1 - rc->stepY) / 2) / rc->rayDirY;
}

void calculate_line_height(int *lineHeight, int *drawStart, int *drawEnd, double perpWallDist) {
    *lineHeight = (int)(S_H / perpWallDist);
    *drawStart = -(*lineHeight) / 2 + S_H / 2;
    if (*drawStart < 0)
        *drawStart = 0;
    *drawEnd = (*lineHeight) / 2 + S_H / 2;
    if (*drawEnd >= S_H)
        *drawEnd = S_H - 1;
}

// void render_column(int x, int drawStart, int drawEnd, int texX, void *tex_ptr, int *buffer, int lineHeight) {
//     for (int y = 0; y < S_H; y++) {
//         if (y < drawStart)
//             buffer[y * S_W + x] = 0x87CEEB; // Sky color
//         else if (y >= drawStart && y <= drawEnd) {
//             int d = y * 256 - S_H * 128 + lineHeight * 128;
//             int texY = ((d * TEX_HEIGHT) / lineHeight) / 256;
//             int color = get_texture_color(tex_ptr, texX, texY);
//             buffer[y * S_W + x] = color;
//         } else {
//             buffer[y * S_W + x] = 0x8B4513; // Ground color
//         }
//     }
// }

void raycasting(t_mlx *mlx, int *buffer) {
    double posX = mlx->ply->plyr_x;
    double posY = mlx->ply->plyr_y;

    for (int x = 0; x < S_W; x++) {
        // Initialize raycasting values
        t_raycast rc;
        rc.rayDirX = mlx->ray->dirX + mlx->ray->planeX * (2 * x / (double)S_W - 1);
        rc.rayDirY = mlx->ray->dirY + mlx->ray->planeY * (2 * x / (double)S_W - 1);
        rc.hit = 0;

        int mapX = (int)posX;
        int mapY = (int)posY;

        // Calculate delta distances
        rc.deltaDistX = fabs(1 / rc.rayDirX);
        rc.deltaDistY = fabs(1 / rc.rayDirY);

        // Calculate step and side distances
        calculate_step_and_side_dist(&rc, posX, posY, mapX, mapY);
        
        // Perform DDA
        perform_dda(&mapX, &mapY, &rc, mlx);

        // Calculate perpendicular wall distance
        rc.perpWallDist = calculate_perp_wall_dist(rc.side, posX, posY, mapX, mapY, &rc);

        // Calculate line height
        int lineHeight, drawStart, drawEnd;
        calculate_line_height(&lineHeight, &drawStart, &drawEnd, rc.perpWallDist);

        // Determine texture X coordinate
        int texX = (int)((rc.side == 0 ? posY + rc.perpWallDist * rc.rayDirY : posX + rc.perpWallDist * rc.rayDirX) * TEX_WIDTH) % TEX_WIDTH;

        // Render the column
        for (int y = 0; y < S_H; y++) {
        if (y < drawStart)
            buffer[y * S_W + x] = 0x87CEEB; // Sky color
        else if (y >= drawStart && y <= drawEnd) {
            int d = y * 256 - S_H * 128 + lineHeight * 128;
            int texY = ((d * TEX_HEIGHT) / lineHeight) / 256;
            int color = get_texture_color(mlx->textures->north, texX, texY);
            buffer[y * S_W + x] = color;
        } else {
            buffer[y * S_W + x] = 0x8B4513; // Ground color
        }
    }
    }
}

char **get_map(char **data)
{
    int i;
    int j;
    int rows;
    char **map;
    int start_line = 6;

    rows = 0;
    while (data[start_line + rows])
        rows++;
    map = malloc(sizeof(char *) * (rows + 1));
    if (!map)
        return (NULL);
    for (i = 0; i < rows; i++)
    {
        map[i] = malloc(sizeof(char) * (ft_strlen(data[start_line + i]) + 1));
        if (!map[i])
            return (NULL);
        j = 0;
        while (data[start_line + i][j])
        {
            map[i][j] = data[start_line + i][j];
            j++;
        }
        map[i][j] = '\0';
    }
    map[rows] = NULL;
    return (map);
}

// t_data *init_argumet(char *av)	// init the data structure
// {
// 	t_data	*dt;
// 	dt = malloc(sizeof(t_data));
// 	// dt-> = read_map(av);
//     // dt->map2d = get_map(dt);
// 	size_map(dt);
// 	// dt->p_y = 3; // player y position in the map
// 	// dt->p_x = 19; // player x position in the map
//  // map height
// 	return (dt); // return the data structure
// }
int	**copy_char_to_int(char **char_matrix, t_data *dt)
{
	int	**int_matrix;
	int	i;
	int	j;

	i = 0;
	int_matrix = (int **)malloc(dt->rows * sizeof(int *));
	if (!int_matrix)
		return (NULL);
	while (i < dt->rows)
	{
		int_matrix[i] = (int *)malloc(dt->cols * sizeof(int));
		if (!int_matrix[i])
			return (NULL);
		j = 0;
		while (j < dt->cols)
		{
			if (ft_isdigit(char_matrix[i][j]))
				int_matrix[i][j] = char_matrix[i][j] - '0';
			else
				int_matrix[i][j] = 0;
			j++;
		}
		i++;
	}
	return (int_matrix);
}

void size_map(t_data *dt) {
    int rows = 0;
    int cols = 0;

    while (dt->map2d[rows]) {
        int current_col = 0;
        while (dt->map2d[rows][current_col]) {
            current_col++;
        }
        if (current_col > cols) {
            cols = current_col;
        }
        rows++;
    }

    dt->rows = rows;
    dt->cols = cols;
}
void print_int_map(int **map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}
void update_map(t_mlx *mlx)
{
    // int buffer[S_W * S_H] = {0};

    if (mlx->img_ptr)
        mlx_destroy_image(mlx->mlx_p, mlx->img_ptr);
    mlx->img_ptr = mlx_new_image(mlx->mlx_p, S_W, S_H);
    draw_buffer(mlx->mlx_p, mlx->win, mlx->img_ptr, mlx->dt->buffer);
    raycasting(mlx, mlx->dt->buffer);
}
int main(int ac, char **av) {
	  if (ac < 2) {
        fprintf(stderr, "Usage: %s <map_file>\n", av[0]);
        return 1;
    }

    void *tex_ptr;
    t_mlx mlx;
    int buffer[S_W * S_H] = {0};
    mlx.ply = calloc(1, sizeof(t_player));
    mlx.ray = calloc(1, sizeof(t_ray));
    mlx.textures = calloc(1, sizeof(t_textures));
    mlx.rc = calloc(1, sizeof(t_raycast));
    mlx.dt = calloc(1, sizeof(t_data));
    mlx.dt->backup = read_map(av[1]);
    mlx.dt->map2d = get_map( mlx.dt->backup);
    size_map(mlx.dt);
    mlx.dt->map = copy_char_to_int(mlx.dt->map2d, mlx.dt);
    // mlx.dt->map = copy_char_to_int(mlx.dt->map2d, 20, 20);
    mlx.mlx_p = mlx_init();
    mlx.win = mlx_new_window(mlx.mlx_p, S_W, S_H, "Cub3d");
    mlx.img_ptr = mlx_new_image(mlx.mlx_p, S_W, S_H); 
    int i, j;
    tex_ptr = mlx_xpm_file_to_image(mlx.mlx_p, "./textures/gumball.xpm", &i, &j);
    if (!tex_ptr) {
        fprintf(stderr, "Error loading texture\n");
        return 1;
    }

    // Set up the initial position, direction, and plane
 
    // mlx.dt->map2d = get_map(mlx.dt);
    // int a = 0;
    // int x = 0;
    print_int_map(mlx.dt->map, mlx.dt->rows, mlx.dt->cols);
    // while ( mlx.dt->map2d[a])
    // {
    //     x = 0;
    //     while (mlx.dt->map2d[a][x])
    //     {
    //         printf("%c", mlx.dt->map2d[a][x]);
    //         x++;

    //     }
    //     printf("\n");
    //     a++;
    // }
    mlx.ply->plyr_x = 10;
    mlx.ply->plyr_y = 5;
    mlx.ray->dirX = -1.0;
    mlx.ray->dirY = -1.0;
    mlx.ray->planeX = 0.0;
    mlx.ray->planeY = 0.66;
    mlx.textures->north = tex_ptr;
	mlx.ply->angle = 0;
    // Main loop
    int done = 0;
    while (!done) {
        // Handle events
        mlx_hook(mlx.win, 17, 0L, (int (*)())exit, NULL); // Close window

        mlx_hook(mlx.win, KeyPress, KeyPressMask, keypress, &mlx);
        raycasting(&mlx, buffer);
        draw_buffer(mlx.mlx_p, mlx.win, mlx.img_ptr, buffer);
        mlx_loop(mlx.mlx_p);
    }
    mlx_destroy_image(mlx.mlx_p, mlx.img_ptr);
    mlx_destroy_image(mlx.mlx_p, tex_ptr);
    mlx_destroy_window(mlx.mlx_p, mlx.win);
    mlx_destroy_display(mlx.mlx_p);
    free(mlx.mlx_p);
    free(mlx.ply);
    free(mlx.ray);
    free(mlx.textures);
    free(mlx.rc);

    return 0;
}