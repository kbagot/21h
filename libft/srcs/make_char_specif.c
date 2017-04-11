/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_char_specif.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 19:16:31 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:14:50 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	spec_s(t_prt *prt, t_flag *flag, va_list ap)
{
	char	*tmp;

	tmp = va_arg(ap, char*);
	if (tmp != NULL)
	{
		prt->spec = ft_strdup(tmp);
		add_prt(prt, flag);
	}
	else if (flag->precision != 0)
	{
		ft_putstr("(null)");
		prt->returnvalue += 6;
	}
	else
	{
		prt->spec = ft_strdup("\0");
		add_prt(prt, flag);
	}
}

static void	spec_c(t_prt *prt, t_flag *flag, va_list ap)
{
	prt->spec = ft_strnew(2);
	prt->spec[0] = va_arg(ap, int);
	if (prt->spec[0] == 0)
	{
		if (flag->width > 0)
			flag->width--;
		prt->returnvalue++;
		if (flag->minus == 1)
		{
			ft_putchar(0);
			add_prt(prt, flag);
		}
		else
		{
			add_prt(prt, flag);
			ft_putchar(0);
		}
	}
	else
		add_prt(prt, flag);
}

static void	spec_lc(t_prt *prt, t_flag *flag, va_list ap)
{
	wint_to_char(va_arg(ap, wint_t), prt, flag);
	add_prt(prt, flag);
}

static void	spec_ls(t_prt *prt, t_flag *flag, va_list ap)
{
	wchar_to_char(va_arg(ap, wchar_t*), prt, flag);
	if (flag->precision != 0)
		flag->precision = -1;
	add_prt(prt, flag);
}

void		make_char_specif(va_list ap, t_prt *prt, t_flag *flag)
{
	if (prt->i == 'S' || (prt->i == 's' && flag->l == 1))
		spec_ls(prt, flag, ap);
	else if (prt->i == 'C' || (prt->i == 'c' && flag->l == 1))
		spec_lc(prt, flag, ap);
	else if (prt->i == 's')
		spec_s(prt, flag, ap);
	else if (prt->i == 'c')
		spec_c(prt, flag, ap);
}
