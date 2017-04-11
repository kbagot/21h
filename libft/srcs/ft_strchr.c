/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 15:32:27 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/21 11:47:16 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	*src;

	src = (unsigned char*)s;
	i = 0;
	while (src[i])
	{
		if (src[i] == (unsigned char)c)
			return ((char *)&src[i]);
		i++;
	}
	if (src[i] == (unsigned char)c)
		return ((char *)&src[i]);
	return (NULL);
}
