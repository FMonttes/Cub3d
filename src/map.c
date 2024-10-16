/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:38:17 by felperei          #+#    #+#             */
/*   Updated: 2024/10/16 14:11:31 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"


// Função para ler o arquivo e retornar o conteúdo como um array de strings
char **read_map(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) return NULL;

    char buffer[1024];
    ssize_t bytes_read;
    char *holder_map = malloc(1);
    if (!holder_map) return NULL;

    holder_map[0] = '\0';

    // Ler o arquivo em partes
    while ((bytes_read = read(fd, buffer, 1023)) > 0) {
        buffer[bytes_read] = '\0';
        char *temp = holder_map;
        holder_map = ft_strjoin(holder_map, buffer);
        free(temp);
        if (!holder_map) return NULL;
    }

    close(fd);
    char **map = ft_split(holder_map, '\n');
    free(holder_map);

    return map;
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

void update_map(t_mlx *mlx)
{
    // int buffer[S_W * S_H] = {0};

    if (mlx->img_ptr)
        mlx_destroy_image(mlx->mlx_p, mlx->img_ptr);
    mlx->img_ptr = mlx_new_image(mlx->mlx_p, S_W, S_H);
    draw_buffer(mlx->mlx_p, mlx->win, mlx->img_ptr, mlx->dt->buffer);
    raycasting(mlx, mlx->dt->buffer);
}
// int	size_map(t_data *dt)
// {
// 	int	i;

// 	dt->w_map = ft_strlen(dt->map2d[0]);
// 	i = 0;
// 	while (dt->map2d[i])
// 		i++;
// 	dt->h_map = i;
// 	if (dt->w_map != dt->h_map)
// 		return (1);
// 	return (0);
// }