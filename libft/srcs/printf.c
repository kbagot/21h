/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 15:15:14 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:27 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	no_specif(t_prt *prt, int j, int pp, const char *restrict format)
{
	char space;

	space = ' ';
	pp = ft_atoi(&format[pp]);
	if (pp < 0)
	{
		pp *= -1;
		prt->returnvalue += pp;
		ft_putchar(format[j - 1]);
		while (--pp > 0)
			ft_putchar(space);
		return (j);
	}
	else if (pp != 0)
	{
		if (format[prt->index + 1] == '0')
			space = '0';
		prt->returnvalue += pp;
		while (--pp > 0)
			ft_putchar(space);
		ft_putchar(format[j - 1]);
		return (j);
	}
	else
		return (j - 1);
}

static int	b_parse(t_prt *prt, va_list ap, int j, const char *restrict format)
{
	int pp;

	pp = j;
	while ((ft_strchr("sSpdiDoOuUxXcC%", format[j])) == NULL && format[j])
		if ((ft_strchr("%*#0-+ hljz.123456789", format[j++])) == NULL)
		{
			j = no_specif(prt, j, pp, format);
			return (j);
		}
	prt->prt = ft_strsub(&format[prt->index], 1, j - prt->index);
	make_specifier(ap, prt);
	free(prt->prt);
	prt->index = j + 1;
	return (prt->index);
}

static int	send_prt(t_prt *prt, int j, va_list ap, const char *restrict format)
{
	int pcharc;

	pcharc = 0;
	while (format[prt->index])
		if (format[prt->index] == '%')
		{
			j = prt->index + 1;
			if (format[j] == '\0')
				return (-1);
			else if (ft_strchr("sSpdiDoOuUxXcC%", format[j]) == NULL)
				prt->index = b_parse(prt, ap, j, format);
			else
			{
				prt->prt = ft_strsub(&format[prt->index], 1, 1);
				make_specifier(ap, prt);
				free(prt->prt);
				prt->index += 2;
			}
		}
		else
		{
			ft_putchar(format[prt->index++]);
			pcharc++;
		}
	return (pcharc);
}

int			ft_printf(const char *restrict format, ...)
{
	t_prt	*prt;
	va_list	ap;
	int		j;
	int		pcharc;

	prt = (t_prt*)malloc(sizeof(t_prt));
	pcharc = 0;
	prt->index = 0;
	j = 0;
	va_start(ap, format);
	prt->returnvalue = 0;
	pcharc = send_prt(prt, j, ap, format);
	va_end(ap);
	free(prt);
	if (pcharc == -1)
		return (0);
	return (prt->returnvalue + pcharc);
}
