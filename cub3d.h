/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:02:17 by ican              #+#    #+#             */
/*   Updated: 2025/10/29 10:52:54 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H


#include "libft/libft.h"
#include "get_next_line/get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

#define FLOOR '0'
#define WALL '1'

typedef struct s_map
{
	char	*name;
	char	**map_lines;
	int		height;
	int		fd;
	int		map_start_index;
	int		map_height;
	char	*one_line;

}	t_map;


typedef struct s_map_comp //map components
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	char	*f;
	char	*c;
	int		floor_color[3];
	int		ceiling_color[3];
}	t_map_comp;

typedef struct s_cub3d
{
	t_map		*map;
	t_map_comp	*comp;
	int			player_x;
	int			player_y;
	char		player_dir;
}	t_cub3d;

typedef struct s_wall
{

}	t_wall;

//utils.c
void	error_msg(char *message, int code, t_cub3d *cub);
int		check_tab(char *line);
int		empty(char c);
char	*trim_spaces(char *str);
char	*extract_path(char *line);
char	*extract_color(char *line, t_cub3d *cub);
void	validate_texture_file(char *path, t_cub3d *cub);
//utils1.c
void	free_map(t_cub3d *cub);
void	free_comp(t_cub3d *cub);
//controller.c
void	arg_controller(char **arg, int ac);
//map_check.c
int		cub_check(char *file_name);
void	check_map_exist(t_cub3d *cub);
void	copy_map(t_cub3d *cub);
int		check_comp(char *line, t_map_comp *comp, t_cub3d *cub);
void	is_map_valid(char **map_lines, t_cub3d *cub);


void	all_free(t_cub3d *cub);
void	eliminate_one_line(t_cub3d *cub);
void	split_one_line(t_cub3d *cub);

//map_check_walls.c
void	check_map_layout(t_cub3d *cub);

#endif