/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width_mod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 13:17:15 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:14:23 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	width_mod_2(t_flag *flag, t_prt *prt, int i)
{
	if (prt->spec[0] == '0')
	{
		while (prt->spec[i] == '0' && prt->spec[i])
			i++;
		if (prt->spec[i] == '+' || prt->spec[i] == '-')
		{
			prt->spec[0] = prt->spec[i];
			prt->spec[i] = '0';
		}
	}
	if (flag->zero == 1 && (flag->hash == 1 || prt->i == 'p'))
	{
		if (prt->i == 'p')
			prt->spec[1] = 'x';
		else
			prt->spec[1] = prt->i;
	}
	if (flag->space == 1)
		prt_modifier(prt, ft_strjoin(" ", prt->spec));
}

void		width_mod(t_flag *flag, t_prt *prt)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strnew(1);
	tmp[0] = 32;
	prt->speclen = (int)ft_strlen(prt->spec);
	if (flag->space == 1)
		flag->width--;
	if (flag->zero == 1 && (flag->hash == 1 || prt->i == 'p'))
		prt->spec[1] = '0';
	while ((flag->width - prt->speclen) > 0)
	{
		if (flag->zero == 1)
			tmp[0] = 48;
		if (flag->minus == 1)
			prt_modifier(prt, ft_strjoin(prt->spec, tmp));
		else
			prt_modifier(prt, ft_strjoin(tmp, prt->spec));
		flag->width--;
	}
	width_mod_2(flag, prt, i);
	free(tmp);
}
