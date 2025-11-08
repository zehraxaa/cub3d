/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:00:41 by aaydogdu          #+#    #+#             */
/*   Updated: 2025/11/08 11:40:56 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	cub_check(char *file_name)
{
	int	i;

	i = ft_strlen(file_name);
	if (file_name[i - 1] == 'b' && file_name[i - 2] == 'u'
		&& file_name[i - 3] == 'c' && file_name[i - 4] == '.')
		return (0);
	else
		return (1);
}

void	check_map_exist(t_cub3d *cub)
{
	int	fd;

	fd = open(cub->map->name, O_RDONLY);
	if (fd < 0)
	{
		close(fd);
		error_msg("File could not found or opened\n", 2, cub);
	}
	cub->map->fd = fd;
}

void	copy_map(t_cub3d *cub)
{
	char	*line;
	int		i;
	int		line_count;

	// Önce dosyadaki satır sayısını say
	line_count = 0;
	while (1)
	{
		line = get_next_line(cub->map->fd, 0);
		if (line == NULL)
			break;
		free(line); // Geçici olarak okunan satırı serbest bırak
		line_count++;
	}
	// get_next_line içindeki statik tamponu sıfırla
	get_next_line(cub->map->fd, 1);
	
	// Dosyayı tekrar aç (fd'yi sıfırla)
	close(cub->map->fd);
	cub->map->fd = open(cub->map->name, O_RDONLY);
	if (cub->map->fd < 0)
		error_msg("File could not be reopened\n", 2, cub);
	
	// map_lines için bellek ayır ve one_line'i başlat
	cub->map->map_lines = (char **)malloc(sizeof(char *) * (line_count + 1));
	if (!cub->map->map_lines)
		error_msg("Memory allocation failed\n", 2, cub);
	// one_line: boş string ile başla
	cub->map->one_line = (char *)malloc(1);
	if (!cub->map->one_line)
		error_msg("Memory allocation failed\n", 2, cub);
	cub->map->one_line[0] = '\0';
	
	// Şimdi satırları oku ve kaydet
	i = 0;
	while (1)
	{
		line = get_next_line(cub->map->fd, 0);
		if (line == NULL)
			break;
		cub->map->map_lines[i] = line;
		// one_line içine ekle
		{
			char *joined;
			joined = ft_strjoin(cub->map->one_line, line);
			if (!joined)
				error_msg("Memory allocation failed\n", 2, cub);
			free(cub->map->one_line);
			cub->map->one_line = joined;
		}
		i++;
	}
	cub->map->map_lines[i] = NULL;
    cub->map->height = i;
	// get_next_line statik tampon temizliği
	get_next_line(cub->map->fd, 1);
	
// Son elemanı NULL yap
}

int	check_comp(char *line, t_map_comp *comp, t_cub3d *cub)
{
	line = trim_spaces(line);
	if (!line || !*line)
		return (0); // Boş satır, hata değil
	if (check_tab(line))
		return (1);
	if (!ft_strncmp(line, "NO", 2) && empty(line[2]) && !comp->no)
	{
		comp->no = extract_path(line + 2);
		if (comp->no)
			validate_texture_file(comp->no, cub);
	}
	else if (!ft_strncmp(line, "SO", 2) && empty(line[2]) && !comp->so)
	{
		comp->so = extract_path(line + 2);
		if (comp->so)
			validate_texture_file(comp->so, cub);
	}
	else if (!ft_strncmp(line, "WE", 2) && empty(line[2]) && !comp->we)
	{
		comp->we = extract_path(line + 2);
		if (comp->we)
			validate_texture_file(comp->we, cub);
	}
	else if (!ft_strncmp(line, "EA", 2) && empty(line[2]) && !comp->ea)
	{
		comp->ea = extract_path(line + 2);
		if (comp->ea)
			validate_texture_file(comp->ea, cub);
	}
	else if (line[0] == 'F' && empty(line[1]) && !comp->f)
		comp->f = extract_color(line + 1, cub); // RGB color parsing
	else if (line[0] == 'C' && empty(line[1]) && !comp->c)
		comp->c = extract_color(line + 1, cub); // RGB color parsing
	else if (!ft_strncmp(line, "NO", 2) || !ft_strncmp(line, "SO", 2) || \
			 !ft_strncmp(line, "WE", 2) || !ft_strncmp(line, "EA", 2) || \
			 line[0] == 'F' || line[0] == 'C')
		return (1); // Hatalı format veya duplicate (tekrarlanan) bileşen
	else
		return (2); // Bu bir bileşen değil, muhtemelen harita satırı
	return (0);
}

static int	all_comps_found(t_map_comp *comp)
{
	if (!comp->no || !comp->so || !comp->we || !comp->ea || \
		!comp->f || !comp->c)
		return (0);
	return (1);
}

void	is_map_valid(char **map_lines, t_cub3d *cub)
{
	int	i;
	int	map_start_index;
	int	comp_status;
	int	map_end_index;

	i = 0;
	map_start_index = -1;
	map_end_index = -1;
	while (map_lines[i])
	{
		comp_status = check_comp(map_lines[i], cub->comp, cub);
		// Harita başlamadan önce yalnız başına '0' kontrolü
		if (map_start_index == -1 && comp_status == 2)
		{
			char	*trimmed;
			
			trimmed = trim_spaces(map_lines[i]);
			// Eğer satır trim edildikten sonra sadece "0" içeriyorsa, bu bir hatadır
			if (trimmed && ft_strlen(trimmed) == 1 && trimmed[0] == '0')
				error_msg("Standalone '0' found outside map definition\n", 1, cub);
		}
		if (comp_status == 1) // 1 = Hata
			error_msg("Invalid component format\n", 1, cub);
		else if (comp_status == 2) // 2 = Harita satırı
		{
			// Harita satırı bulduk, ama önce tüm bileşenler bulunmuş olmalı
			if (!all_comps_found(cub->comp))
				error_msg("Map line found before all components were set\n", 1, cub);
			// Bu, haritanın başladığı ilk satır
			if (map_start_index == -1)
				map_start_index = i;
			// Haritanın bittiği son satır (son harita satırını bul)
			// Not: Boş satırlardan sonra tekrar harita gelirse, bu da haritanın devamıdır
			map_end_index = i;
		}
		// Eğer harita başladıysa ve boş satır görürsek
		else if (comp_status == 0 && map_start_index != -1)
		{
			// Boş satır: Eğer map_end_index set edilmişse (yani harita karakterleri görüldüyse)
			// bu harita içindeki boş satırdır - kontrol için map_end_index'i genişlet
			// Böylece boş satır da kontrol edilecek
			if (map_end_index != -1)
				map_end_index = i; // Boş satırı da harita aralığına dahil et (kontrol için)
		}
		i++;
	}
	// Döngü bitti, son kontroller
	if (!all_comps_found(cub->comp))
		error_msg("Missing one or more map components\n", 1, cub);
	if (map_start_index == -1)
		error_msg("No map found in file\n", 1, cub);

	// Harita başlangıç indeksini kaydet
	cub->map->map_start_index = map_start_index;
	// Haritanın gerçek yüksekliğini kaydet (map_end_index dahil)
	cub->map->map_height = map_end_index - map_start_index + 1;
	
	// Harita içinde boş satır var mı kontrol et
	// Component'ler ile harita arasındaki boş satırlar zaten izin veriliyor (comp_status == 0)
	// Ama harita başladıktan sonra (map_start_index'ten sonra) boş satır olamaz
	i = map_start_index;
	while (i <= map_end_index)
	{
		char	*trimmed;
		int		is_empty;
		
		// Satırın boş olup olmadığını kontrol et
		trimmed = trim_spaces(map_lines[i]);
		is_empty = (!trimmed || !*trimmed);
		
		// Eğer satır boşsa, bu harita içindeki boş satırdır - HATA
		if (is_empty)
			error_msg("Empty line inside map definition\n", 1, cub);
		i++;
	}
	
	// Harita sonrasında geçersiz içerik var mı kontrol et
	// Harita sonrasında sadece boş satırlar olabilir, başka bir şey olmamalı
	i = map_end_index + 1;
	while (map_lines[i])
	{
		char	*trimmed;
		
		trimmed = trim_spaces(map_lines[i]);
		// Eğer harita sonrasında boş olmayan bir satır varsa, bu geçersizdir
		if (trimmed && *trimmed)
			error_msg("Invalid content after map definition\n", 1, cub);
		i++;
	}
	
	// Şimdi harita içeriğini (duvarlar, oyuncu, kapalılık) kontrol et
	check_map_layout(cub);
}

static int	parse_rgb(char *line)
{
    // Bu fonksiyon tek bir renk değerini (örn: "220") kontrol eder
    int i = 0;
    while (line[i] && line[i] == ' ') // Baştaki boşlukları atla
        i++;
    if (!line[i]) return (-1); // Sadece boşluksa hata
    int j = i;
    while (line[j] && ft_isdigit(line[j])) // Rakamları kontrol et
        j++;
    while (line[j] && line[j] == ' ') // Sondaki boşlukları atla
        j++;
    if (line[j] != '\0') return (-1); // Rakam ve boşluk dışında bir şey varsa hata
    int val = ft_atoi(line + i);
    if (val < 0 || val > 255) return (-1); // 0-255 aralığı kontrolü
    return (val);
}

void validate_colors(t_cub3d *cub)
{
    char **rgb;
    int i;

    // Zemin (Floor) Rengi Kontrolü
    rgb = ft_split(cub->comp->f, ',');
    if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3]) // Tam 3 parça olmalı
        error_msg("Invalid floor color format\n", 1, cub); // rgb'yi free'lemeyi unutma!
    i = -1;
    while (++i < 3)
    {
        cub->comp->floor_color[i] = parse_rgb(rgb[i]);
        if (cub->comp->floor_color[i] == -1)
             error_msg("Invalid floor color value (0-255 only)\n", 1, cub); // rgb'yi free'le!
    }
    // rgb dizisini free'le (libft'inde ft_free_split gibi bir şey varsa kullan)

    // Tavan (Ceiling) Rengi Kontrolü
    // (Aynı işlemi cub->comp->c ve cub->comp->ceiling_color için tekrarla)
}
