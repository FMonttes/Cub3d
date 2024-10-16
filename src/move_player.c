/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:13:21 by felperei          #+#    #+#             */
/*   Updated: 2024/10/15 15:06:29 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void move_player_forward(t_mlx *mlx) {

	int newX = (int)((mlx->ply->plyr_y ) / TILE_SIZE);
	int newY = (int)((mlx->ply->plyr_x ) / TILE_SIZE);

	if (newX >= 0 && newX < mlx->dt->h_map && newY >= 0 && newY < mlx->dt->w_map) {
		if (mlx->dt->map[newX][newY] != 1) {
			mlx->ply->plyr_x += 1 * PLAYER_SPEED;
		}
	}
}

void move_player_backward(t_mlx *mlx) {
    double move_x = -cos(mlx->ply->angle) * PLAYER_SPEED;
    double move_y = -sin(mlx->ply->angle) * PLAYER_SPEED;

    int newX = (int)((mlx->ply->plyr_y + move_y) / TILE_SIZE);
    int newY = (int)((mlx->ply->plyr_x + move_x) / TILE_SIZE);

    if (newX >= 0 && newX <  mlx->dt->h_map  && newY >= 0 && newY < mlx->dt->w_map ) {
        if (mlx->dt->map[newX][newY] != 1) {
            mlx->ply->plyr_x += move_x;
            mlx->ply->plyr_y += move_y;
        }
    }
}


void	rotate_player(t_mlx *mlx, int i)	// rotate the player
{
	if (i == 1)
	{
		mlx->ply->angle += ROTATION_SPEED; // rotate right
		if (mlx->ply->angle > 2 * M_PI)
			mlx->ply->angle -= 2 * M_PI;
	}
	else
	{
		mlx->ply->angle -= ROTATION_SPEED; // rotate left
		if (mlx->ply->angle < 0)
			mlx->ply->angle += 2 * M_PI;
	}
}


int	keypress(int keycode, t_mlx *game)
{
	if (keycode == ESC || keycode == KEY_Q)
		ft_exit(game);
	game_events(keycode, game);
	return (0);
}

void	game_events(int keycode, t_mlx *game)
{
	if (keycode == KEY_W || keycode == KEY_UP)
	{
		move_player_forward(game);
		printf("W\n");
		update_map(game);
	}
	else if (keycode == KEY_S || keycode == KEY_DOWN)
	{
		move_player_backward(game);
		printf("S\n");
	}
	else if (keycode == KEY_D || keycode == KEY_RIGHT)
	{
		// game->ply->rot = 1;
		rotate_player(game, 1);
	}
	else if (keycode == KEY_A || keycode == KEY_LEFT)
	{
		// game->ply->rot = -1;
		rotate_player(game, 0);
	}
}
