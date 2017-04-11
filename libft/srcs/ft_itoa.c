/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 14:39:35 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/15 15:55:44 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nb(int n)
{
	int				nb;
	unsigned int	nnb;

	nnb = (unsigned int)n;
	nb = 0;
	if (n < 0)
	{
		nb = 1;
		nnb = n * -1;
	}
	if (n == 0)
		return (1);
	while (nnb > 0)
	{
		nnb = nnb / 10;
		nb++;
	}
	return (nb);
}

char		*ft_itoa(int n)
{
	int				i;
	char			*dest;
	int				signe;
	unsigned int	nb;

	i = 0;
	nb = (unsigned int)n;
	signe = 0;
	i = ft_nb(n) - 1;
	if ((dest = ft_strnew(ft_nb(n))) == NULL)
		return (NULL);
	if (n < 0)
	{
		nb = n * -1;
		signe = 1;
	}
	while (i >= 0)
	{
		dest[i] = nb % 10 + 48;
		i--;
		nb = nb / 10;
	}
	if (signe == 1)
		dest[0] = '-';
	return (dest);
}
