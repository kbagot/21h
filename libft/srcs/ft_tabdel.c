/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/04 18:47:31 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/19 19:05:31 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_tabdel(char ***s)
{
	int i;

	i = 0;
	if (*s)
	{
		while ((*s)[i])
		{
			ft_strdel(&(*s)[i]);
			i++;
		}
		free(*s);
		*s = NULL;
	}
}
