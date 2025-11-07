/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:27:02 by aaydogdu          #+#    #+#             */
/*   Updated: 2024/11/07 14:35:30 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	dest_len = 0;
	src_len = 0;
	while (dst[dest_len] != '\0' && dest_len < dstsize)
		dest_len++;
	while (src[src_len] != '\0')
		src_len++;
	if (dstsize <= dest_len)
		return (dstsize + src_len);
	i = 0;
	while (src[i] != '\0' && (dest_len + i + 1) < dstsize)
	{
		dst[dest_len + i] = src[i];
		i++;
	}
	dst[dest_len + i] = '\0';
	return (dest_len + src_len);
}
