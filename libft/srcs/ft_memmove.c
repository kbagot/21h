/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 14:02:38 by kbagot            #+#    #+#             */
/*   Updated: 2017/03/03 10:00:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = (char*)dest;
	s = (const char*)src;
	if (s < d)
	{
		s = s + n - 1;
		d = d + n - 1;
		while (n > 0)
		{
			*d-- = *s--;
			n--;
		}
	}
	while (n > 0)
	{
		*d++ = *s++;
		n--;
	}
	return (dest);
}
