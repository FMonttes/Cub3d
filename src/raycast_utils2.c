/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:09:33 by fmontes           #+#    #+#             */
/*   Updated: 2024/10/16 14:10:33 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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