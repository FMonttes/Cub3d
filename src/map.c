/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontes <fmontes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:38:17 by felperei          #+#    #+#             */
/*   Updated: 2024/10/15 13:35:29 by fmontes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

#include <fcntl.h>  // Para open
#include <unistd.h> // Para read, close
#include <stdlib.h> // Para malloc, free



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