/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_walls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 10:46:27 by aaydogdu          #+#    #+#             */
/*   Updated: 2025/10/29 12:53:49 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_empty(t_cub3d *cub, int y, int x)
{
	char	**map;
	int		start;
	int		height;

	map = cub->map->map_lines;
	start = cub->map->map_start_index;
	height = cub->map->map_height;

	// 1. Harita sınırları (yukarı/aşağı) kontrolü
	if (y < 0 || y >= height)
		return (1); // Sınır dışı = boş kabul edilir

	// 2. Satır sınırları (sol/sağ) kontrolü
	// Önce satırın varlığını kontrol et (segfault önlemek için)
	if (!map[start + y]) return (1);
	if (x < 0 || (int)ft_strlen(map[start + y]) <= x)
		return (1); // Satır sonu veya öncesi = boş kabul edilir

	// 3. Karakter kontrolü
	if (map[start + y][x] == ' ' || map[start + y][x] == '\n' || map[start + y][x] == '\0')
		return (1);

	return (0); // Dolu (0, 1, N, S, E, W)
}

static void	check_stray_wall(t_cub3d *cub, int y, int x)
{
	if (is_empty(cub, y - 1, x) && // Yukarı
		is_empty(cub, y + 1, x) && // Aşağı
		is_empty(cub, y, x - 1) && // Sol
		is_empty(cub, y, x + 1))   // Sağ
	{
		error_msg("Stray (orphaned) wall detected\n", 1, cub);
	}
}

// Flood-fill için haritanın bir kopyasını oluşturur
static char	**duplicate_map(t_cub3d *cub)
{
	char	**copy;
	int		i;
	int		start;

	start = cub->map->map_start_index;
	copy = (char **)malloc(sizeof(char *) * (cub->map->map_height + 1));
	if (!copy)
		error_msg("Memory allocation failed for map copy\n", 1, cub);
	i = 0;
	while (i < cub->map->map_height)
	{
		copy[i] = ft_strdup(cub->map->map_lines[start + i]);
		if (!copy[i])
		{
			// Hata durumunda o ana kadar kopyalananları temizle
			while (i > 0)
				free(copy[--i]);
			free(copy);
			error_msg("Memory allocation failed for map copy line\n", 1, cub);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

// Harita kopyasını temizler
static void	free_map_copy(char **map_copy)
{
	int	i;

	i = 0;
	while (map_copy[i])
	{
		free(map_copy[i]);
		i++;
	}
	free(map_copy);
}
static void	validate_chars_and_find_player(t_cub3d *cub)
{
	int		i;
	int		j;
	int		player_count;
	int		start;

	player_count = 0;
	start = cub->map->map_start_index;
	i = 0; // i, haritadaki y pozisyonudur (0'dan başlar)
	while (i < cub->map->map_height)
	{
		char *line = cub->map->map_lines[start + i];
		j = 0;
		// ÖNEMLİ: Satır sonundaki \n'i kontrol etme, sadece karakterleri et.
		while (line[j] && line[j] != '\n')
		{
			char c = line[j];
			
			// 1. OYUNCU VEYA ZEMİN İSE YENİ KONTROL
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				// YENİ EKLENEN KONTROL: Komşuları ' ' mı?
				check_stray_wall(cub, i, j);
			}
			
			// 2. Oyuncu tespiti (Bu kısım aynı kalır)
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				player_count++;
				cub->player_x = j;
				cub->player_y = i; 
				cub->player_dir = c;
			}
			// 3. Geçersiz Karakter Kontrolü (Bu kısım aynı kalır)
			else if (c != '0' && c != '1' && c != ' ')
				error_msg("Invalid character in map\n", 1, cub);
			j++;
		}
		
		// Harita içinde boş satır kontrolü (Bu kısım aynı kalır)
		if (j == 0 && (line[j] == '\n' || line[j] == '\0'))
			error_msg("Empty line inside map definition\n", 1, cub);
			
		i++;
	}
	// 4. Oyuncu Sayısı Kontrolü (Bu kısım aynı kalır)
	if (player_count != 1)
		error_msg("Map must have exactly one player start position\n", 1, cub);
}

// Esas Flood-Fill algoritması
// Haritanın "açık" olduğu bir yere (sınır veya ' ') ulaşırsa hata verir
static void	flood_fill(t_cub3d *cub, char **map_copy, int y, int x)
{
	// 1. Sınır Kontrolü (Boundary Check)
	// Eğer haritanın sınırlarının dışına çıkarsak VEYA bir boşluğa (' ') ulaşırsak
	// harita kapalı değildir.
	if (y < 0 || y >= cub->map->map_height || \
		x < 0 || !map_copy[y] || x >= (int)ft_strlen(map_copy[y]) || \
		map_copy[y][x] == ' ')
	{
		// Hata vermeden önce kopyayı temizle
		free_map_copy(map_copy);
		error_msg("Map is not closed (open to space or edge)\n", 1, cub);
	}

	// 2. Durma Koşulu (Stop Condition)
	// Eğer duvara ('1') veya daha önce ziyaret ettiğimiz bir yere ('F') gelirsek dur
	if (map_copy[y][x] == '1' || map_copy[y][x] == 'F')
		return ;

	// 3. İşaretleme (Marking)
	// Ziyaret ettiğimiz yeri 'F' (Filled) olarak işaretle
	map_copy[y][x] = 'F';

	// 4. Yenileme (Recurse)
	// Dört yöne doğru devam et
	flood_fill(cub, map_copy, y + 1, x); // Aşağı
	flood_fill(cub, map_copy, y - 1, x); // Yukarı
	flood_fill(cub, map_copy, y, x + 1); // Sağ
	flood_fill(cub, map_copy, y, x - 1); // Sol
}

// Harita içeriğini (geçerli karakterler, oyuncu sayısı) kontrol eder

// Harita kontrollerini başlatan ana fonksiyon
void	check_map_layout(t_cub3d *cub)
{
	char	**map_copy;

	// 1. Geçersiz karakter var mı? Tam olarak 1 oyuncu var mı?
	// Bu fonksiyon ayrıca cub->player_x/y/dir değerlerini de atar
	validate_chars_and_find_player(cub);

	// 2. Flood-fill için haritanın bir kopyasını oluştur
	map_copy = duplicate_map(cub);

	// 3. Flood-fill'i oyuncunun başladığı yerden başlat
	// Not: player_y, kopyalanmış haritaya (0'dan başlayan) göre ayarlandı
	flood_fill(cub, map_copy, cub->player_y, cub->player_x);

	// 4. Kontrol başarılıysa, kopya haritayı temizle
	free_map_copy(map_copy);

	// Eğer buraya kadar geldiyse harita geçerlidir.
}