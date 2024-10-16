#include "../cub3d.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



void print_int_map(int **map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
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