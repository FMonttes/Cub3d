/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:08:39 by fmontes           #+#    #+#             */
/*   Updated: 2024/10/16 14:09:17 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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