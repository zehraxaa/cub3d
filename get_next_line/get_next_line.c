/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 11:57:48 by aaydogdu          #+#    #+#             */
/*   Updated: 2024/12/15 11:57:48 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*getbefore(char *str)
{
	int		i;
	char	*before_nl;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] != '\n' && str[i])
		i++;
	before_nl = (char *)malloc(sizeof(char) * (i + 2));
	if (!before_nl)
		return (NULL);
	i = 0;
	while (str[i] != '\n' && str[i])
	{
		before_nl[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		before_nl[i] = '\n';
		i++;
	}
	before_nl[i] = '\0';
	return (before_nl);
}

char	*getafter(char *str)
{
	char	*after_nl;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	after_nl = (char *)malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	if (!after_nl)
		return (NULL);
	i++;
	j = 0;
	while (str[i])
		after_nl[j++] = str[i++];
	after_nl[j] = '\0';
	free(str);
	return (after_nl);
}

char	*joinfunc(char *str, char *buffer, int fd)
{
	int	count;

	count = 1;
	while (!ft_strchr(str, '\n') && count != 0)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			free(str);
			free(buffer);
			return (NULL);
		}
		buffer[count] = '\0';
		str = gnl_strjoin(str, buffer);
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd, int i)
{
	char		*output;
	char		*buffer;
	static char	*str;

	if (i == 1)
	{
		if (str)
			free(str);
		return (0);
	}
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	str = joinfunc(str, buffer, fd);
	if (!str)
		return (NULL);
	output = getbefore(str);
	str = getafter(str);
	return (output);
}
