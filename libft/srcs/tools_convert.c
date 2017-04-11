/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_specifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 18:25:40 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:40 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	stock_int(t_prt *prt, int c)
{
	prt->spec[prt->ispec] = c;
	prt->ispec++;
}

void		ft_putoctal(unsigned long long int n, t_prt *prt)
{
	if (n >= 8)
		ft_putoctal(n / 8, prt);
	stock_int(prt, (n % 8) + 48);
}

void		ft_puthexa(unsigned long long int n, char c, t_prt *prt)
{
	int ccase;
	int length;

	length = 0;
	if (c == 'X')
		ccase = 55;
	else
		ccase = 87;
	if (n >= 16)
		ft_puthexa(n / 16, c, prt);
	if (n % 16 <= 9)
		stock_int(prt, (n % 16) + 48);
	else
		stock_int(prt, (n % 16) + ccase);
}

void		ft_masterputnbru(unsigned long long int n, t_prt *prt)
{
	if (n >= 10)
		ft_masterputnbru(n / 10, prt);
	stock_int(prt, (n % 10) + 48);
}

void		ft_masterputnbr(long long int n, t_prt *prt)
{
	unsigned long long int nb;

	nb = n;
	if (n < 0)
	{
		stock_int(prt, '-');
		nb *= -1;
	}
	if (nb >= 10)
		ft_masterputnbr(nb / 10, prt);
	stock_int(prt, (nb % 10) + 48);
}
