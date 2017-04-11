/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_length.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 18:52:53 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:12:19 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	decimal_maker(t_flag *flag, t_prt *prt, va_list ap)
{
	if (flag->l == 1 || prt->i == 'D')
		ft_masterputnbr(va_arg(ap, long int), prt);
	else if (flag->z == 1)
		ft_masterputnbr(va_arg(ap, size_t), prt);
	else if (flag->h == 1)
		ft_masterputnbr((short int)va_arg(ap, int), prt);
	else if (flag->h == 2)
		ft_masterputnbr((signed char)va_arg(ap, int), prt);
	else if (flag->l == 2 || flag->j == 1)
		ft_masterputnbr(va_arg(ap, long long int), prt);
	else
		ft_masterputnbr(va_arg(ap, int), prt);
}

static void	posdeci_maker(t_flag *flag, t_prt *prt, va_list ap)
{
	if (flag->l == 1 || prt->i == 'U')
		ft_masterputnbru(va_arg(ap, unsigned long int), prt);
	else if (flag->z == 1)
		ft_masterputnbru(va_arg(ap, size_t), prt);
	else if (flag->h == 2)
		ft_masterputnbr((unsigned char)va_arg(ap, int), prt);
	else if (flag->l == 2 || flag->j == 1)
		ft_masterputnbru(va_arg(ap, unsigned long long int), prt);
	else if (flag->h == 1)
		ft_masterputnbr((unsigned short int)va_arg(ap, int), prt);
	else
		ft_masterputnbr(va_arg(ap, unsigned int), prt);
}

static void	hexa_maker(t_flag *flag, t_prt *prt, va_list ap)
{
	if (prt->i == 'p')
	{
		ft_strcpy(prt->spec, "0x");
		prt->ispec += 2;
		ft_puthexa(va_arg(ap, unsigned long long int), prt->i, prt);
	}
	else if (flag->z == 1)
		ft_puthexa(va_arg(ap, size_t), prt->i, prt);
	else if (flag->h == 2)
		ft_puthexa((unsigned char)va_arg(ap, int), prt->i, prt);
	else if (flag->h == 1)
		ft_puthexa((unsigned short int)va_arg(ap, int), prt->i, prt);
	else if (flag->l == 1)
		ft_puthexa(va_arg(ap, unsigned long int), prt->i, prt);
	else if (flag->l == 2 || flag->j == 1)
		ft_puthexa(va_arg(ap, unsigned long long int), prt->i, prt);
	else
		ft_puthexa(va_arg(ap, unsigned int), prt->i, prt);
}

static void	octal_maker(t_flag *flag, t_prt *prt, va_list ap)
{
	if (flag->l == 1 || prt->i == 'O')
		ft_putoctal(va_arg(ap, unsigned long int), prt);
	else if (flag->z == 1)
		ft_putoctal(va_arg(ap, size_t), prt);
	else if (flag->h == 2)
		ft_putoctal((unsigned char)va_arg(ap, int), prt);
	else if (flag->h == 1)
		ft_putoctal((unsigned short int)va_arg(ap, int), prt);
	else if (flag->l == 2 || flag->j == 1)
		ft_putoctal(va_arg(ap, unsigned long long int), prt);
	else
		ft_putoctal(va_arg(ap, unsigned int), prt);
}

void		add_length(t_flag *flag, t_prt *prt, va_list ap)
{
	if (prt->i == 'd' || prt->i == 'D' || prt->i == 'i')
		decimal_maker(flag, prt, ap);
	if (prt->i == 'u' || prt->i == 'U')
		posdeci_maker(flag, prt, ap);
	if (prt->i == 'x' || prt->i == 'X' || prt->i == 'p')
		hexa_maker(flag, prt, ap);
	if (prt->i == 'o' || prt->i == 'O')
		octal_maker(flag, prt, ap);
	add_prt(prt, flag);
}
