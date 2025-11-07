/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:01:21 by aaydogdu          #+#    #+#             */
/*   Updated: 2024/11/08 12:14:53 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	wordcount(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**newstr;
	int		i;
	int		j;
	int		size;

	if (!s)
		return (NULL);
	size = wordcount(s, c);
	newstr = malloc(sizeof(char *) * (size + 1));
	if (!newstr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		while (*s == c)
			s++;
		j = 0;
		while (s[j] != c && s[j])
			j++;
		newstr[i++] = ft_substr(s, 0, j);
		s = s + j;
	}
	newstr[i] = NULL;
	return (newstr);
}
