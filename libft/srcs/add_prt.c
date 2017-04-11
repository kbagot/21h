/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_prt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 18:57:33 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:12:27 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		prt_modifier(t_prt *prt, char *new)
{
	ft_strdel(&prt->spec);
	prt->spec = new;
	new = NULL;
}

static void	hash_mod(t_prt *prt)
{
	if (prt->i == 'x' || prt->i == 'X')
	{
		prt_modifier(prt, ft_strjoin(&prt->prt[ft_strlen(prt->prt) - 1],
					prt->spec));
		prt_modifier(prt, ft_strjoin("0", prt->spec));
	}
	else
		prt_modifier(prt, ft_strjoin("0", prt->spec));
}

static void	add_prt2(t_prt *prt, t_flag *flag)
{
	if (flag->space == 1 && (prt->spec[0] != '-' && prt->spec[0] != '+')
			&& ft_strchr("CSoxXOpscu%", prt->i) == NULL &&
			(prt->speclen >= flag->width && prt->speclen >= flag->precision))
	{
		flag->space = 0;
		prt_modifier(prt, ft_strjoin(" ", prt->spec));
	}
	if (flag->plus == 1 && (ft_strchr("di", prt->i)) && prt->spec[0] != '-')
	{
		if (prt->spec[0] == 0)
			prt->spec[0] = '+';
		else
			prt_modifier(prt, ft_strjoin("+", prt->spec));
	}
	if (ft_strchr("Cc%", prt->i) == NULL && flag->precision != -1)
		precision_mod(flag, prt);
	prt->speclen = (int)ft_strlen(prt->spec);
	if (flag->width > prt->speclen)
		width_mod(flag, prt);
	ft_putstr(prt->spec);
}

void		add_prt(t_prt *prt, t_flag *flag)
{
	if (flag->hash == 1 && flag->precision == 0 &&
			ft_atoi(prt->spec) == 0 && ft_strchr("oO", prt->i))
		flag->precision = -1;
	if (ft_strchr("cCsSuUdDioOxXp%", prt->prt[0]))
		ft_putstr(prt->spec);
	else
	{
		if (ft_strchr("cCsSuUDdixXpoO%", prt->i))
		{
			if (flag->hash == 1 && (ft_strchr("oOxX", prt->i)) &&
					ft_atoi(prt->spec) != 0)
				hash_mod(prt);
			prt->speclen = (int)ft_strlen(prt->spec);
			add_prt2(prt, flag);
		}
	}
	prt->returnvalue += ft_strlen(prt->spec);
	ft_strdel(&prt->spec);
}
