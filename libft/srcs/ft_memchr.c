/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 14:03:28 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/14 12:43:54 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*src;
	unsigned int	cc;

	cc = (unsigned char)c;
	src = (unsigned char*)s;
	i = 0;
	while (n > 0)
	{
		if (src[i] == cc)
			return (&src[i]);
		i++;
		n--;
	}
	return (NULL);
}
