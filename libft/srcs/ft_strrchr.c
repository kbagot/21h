/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 12:26:18 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/16 19:56:16 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	*src;

	src = (unsigned char*)s;
	i = 0;
	while (src[i])
		i++;
	while (i > 0 && src[i] != (unsigned char)c)
		i--;
	if (src[i] == (unsigned char)c)
		return ((char *)&src[i]);
	return (NULL);
}
