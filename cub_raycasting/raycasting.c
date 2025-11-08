/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:38:59 by aaydogdu          #+#    #+#             */
/*   Updated: 2025/11/08 11:50:23 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	raycasting(t_cub3d *cub)
{
	int	x;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		raycasting_loop(cub, x);
		x++; 
	}
}

void	raycasting_loop(t_cub3d *cub, int x)
{
	double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	double ray_dir_x = cub->player.dir_x + cub->player.plane_x * camera_x;
	double ray_dir_y = cub->player.dir_y + cub->player.plane_y * camera_x;
}