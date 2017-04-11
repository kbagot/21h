/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precision_mod.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/09 13:24:18 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:17 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	precision_mod_2(t_flag *flag, t_prt *prt, int signe)
{
	if (flag->precision == 0 && ft_atoi(prt->spec) == 0)
		prt->spec[0] = '\0';
	else if (prt->i == 's' || prt->i == 'S')
		prt->spec[flag->precision] = '\0';
	else if (flag->precision > prt->speclen)
		while ((flag->precision - prt->speclen) > 0)
		{
			prt_modifier(prt, ft_strjoin("0", prt->spec));
			flag->precision--;
		}
	if (prt->i == 'p' || ((prt->i == 'X' || prt->i == 'x')
				&& flag->hash == 1 && flag->precision != 0))
	{
		if (prt->i != 'X')
			prt_modifier(prt, ft_strjoin("0x", prt->spec));
		else
			prt_modifier(prt, ft_strjoin("0X", prt->spec));
	}
	if (signe == 45)
		prt_modifier(prt, ft_strjoin("-", prt->spec));
	if (signe == 43)
		prt_modifier(prt, ft_strjoin("+", prt->spec));
}

void		precision_mod(t_flag *flag, t_prt *prt)
{
	int signe;

	signe = 0;
	if (flag->precision != 0)
		flag->zero = 0;
	if (prt->i == 'p' || ((prt->i == 'X' || prt->i == 'x') &&
				flag->hash == 1 && flag->precision != 0))
		prt->spec = ft_strdup(&prt->spec[2]);
	if ((prt->spec[0] == '-' || prt->spec[0] == '+')
			&& flag->precision >= prt->speclen)
	{
		signe = prt->spec[0];
		prt->spec[0] = '0';
	}
	prt->speclen = (int)ft_strlen(prt->spec);
	precision_mod_2(flag, prt, signe);
}
