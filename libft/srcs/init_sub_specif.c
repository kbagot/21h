/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sub_specif.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/02 20:59:11 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:12:50 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	init_var(t_flag *flag)
{
	flag->minus = 0;
	flag->plus = 0;
	flag->zero = 0;
	flag->hash = 0;
	flag->space = 0;
	flag->width = 0;
	flag->precision = -1;
	flag->l = 0;
	flag->h = 0;
	flag->j = 0;
	flag->z = 0;
}

static int	init_flag(t_flag *flag, t_prt *prt, int j)
{
	while (ft_strchr(" -+#0", prt->prt[j]))
	{
		if (prt->prt[j] == '#')
			flag->hash = 1;
		if (prt->prt[j] == ' ')
			flag->space = 1;
		if (prt->prt[j] == '-')
			flag->minus = 1;
		if (prt->prt[j] == '+')
			flag->plus = 1;
		if (prt->prt[j] == '0')
			flag->zero = 1;
		j++;
	}
	if (flag->plus == 1 && flag->space == 1)
		flag->space = 0;
	return (j);
}

static int	init_width_preci(t_flag *flag, t_prt *prt, int j, va_list ap)
{
	if (ft_strchr("*.0123456789", prt->prt[j]))
	{
		if (ft_strchr("0123456789", prt->prt[j]))
			flag->width = ft_atoi(&prt->prt[j]);
		else if (prt->prt[j] == '*')
			flag->width = va_arg(ap, int);
		while (ft_strchr("*0123456789", prt->prt[j]))
			j++;
		if (prt->prt[j] == '.' && ft_strchr("*0123456789", prt->prt[j + 1]))
		{
			if (prt->prt[j + 1] == '*')
				flag->precision = va_arg(ap, int);
			else
				flag->precision = ft_atoi(&prt->prt[j + 1]);
		}
		else if (prt->prt[j] == '.')
			flag->precision = 0;
	}
	while (ft_strchr("*.0123456789", prt->prt[j]))
		j++;
	return (j);
}

static void	init_length(t_flag *flag, t_prt *prt, int j)
{
	while (ft_strchr("hljz", prt->prt[j]))
	{
		if (prt->prt[j] == 'l')
			flag->l++;
		if (prt->prt[j] == 'h')
			flag->h++;
		if (prt->prt[j] == 'j')
			flag->j++;
		if (prt->prt[j] == 'z')
			flag->z++;
		j++;
	}
}

void		init_sub_specif(t_flag *flag, t_prt *prt, va_list ap)
{
	int j;

	j = 0;
	init_var(flag);
	j = init_flag(flag, prt, j);
	j = init_width_preci(flag, prt, j, ap);
	if (flag->minus == 1 && flag->zero == 1)
		flag->zero = 0;
	if (flag->precision < 0)
		flag->precision = -1;
	if (flag->width < 0)
	{
		flag->width *= -1;
		flag->minus = 1;
	}
	init_length(flag, prt, j);
}
