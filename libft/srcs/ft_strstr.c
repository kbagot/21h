/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 13:31:59 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/14 17:53:02 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	char	*b;
	char	*l;
	int		i;
	int		j;
	int		save;

	i = 0;
	b = (char*)big;
	l = (char*)little;
	if (l[i] == '\0')
		return (b);
	while (b[i])
	{
		j = 0;
		save = i;
		while (b[i] == l[j])
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
