/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 15:10:54 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/16 20:02:18 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	int		max;
	int		j;

	i = ft_strlen(dst);
	max = (size / sizeof(char)) - i - 1;
	j = 0;
	if (size > i)
		size = i + ft_strlen(src);
	else
		size = size + ft_strlen(src);
	while (j < max && src[j] != '\0')
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (size);
}
