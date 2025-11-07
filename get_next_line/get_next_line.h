/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaydogdu <aaydogdu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 11:57:36 by aaydogdu          #+#    #+#             */
/*   Updated: 2024/12/15 11:57:36 by aaydogdu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char		*ft_strchr(const char *str, int ch);
char		*get_next_line(int fd, int i);
char		*getbefore(char *str);
char		*getafter(char *str);
char		*joinfunc(char *str, char *buffer, int fd);
char		*gnl_strjoin(char *s1, char *s2);
size_t		ft_strlen(const char *s);

#endif