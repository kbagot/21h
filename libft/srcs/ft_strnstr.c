/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 15:27:05 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/14 17:54:40 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*b;
	char	*l;
	size_t	i;
	int		j;
	int		save;

	i = 0;
	b = (char*)big;
	l = (char*)little;
	if (l[i] == '\0')
		return (b);
	while (b[i] && i < len)
	{
		j = 0;
		save = i;
		while (b[i] == l[j] && i < len)
		{
			i++;
			j++;
			if (l[j] == '\0')
				return (&b[save]);
		}
		i = save + 1;
	}
	return (NULL);
}
