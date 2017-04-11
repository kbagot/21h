/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 15:05:16 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/13 12:21:25 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(dest);
	while (src[i] && n > 0)
	{
		dest[j] = src[i];
		j++;
		i++;
		n--;
	}
	dest[j] = '\0';
	return (dest);
}
