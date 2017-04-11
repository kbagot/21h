/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_convert_uni.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 18:29:24 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:51 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strwchar(int chr, char *str, int i)
{
	int len;

	len = ft_wcharlen(chr);
	if (len == 1)
		str[i++] = chr;
	else if (len == 2)
	{
		str[i++] = (chr >> 6) + 0xC0;
		str[i++] = (chr & 0x3F) + 0x80;
	}
	else if (len == 3)
	{
		str[i++] = (chr >> 12) + 0xE0;
		str[i++] = ((chr >> 6) & 0x3F) + 0x80;
		str[i++] = (chr & 0x3F) + 0x80;
	}
	else if (len == 4)
	{
		str[i++] = (chr >> 18) + 0xF0;
		str[i++] = ((chr >> 12) & 0x3F) + 0x80;
		str[i++] = ((chr >> 6) & 0x3F) + 0x80;
		str[i++] = (chr & 0x3F) + 0x80;
	}
	return (i);
}

void		wint_to_char(wint_t wchar, t_prt *prt, t_flag *flag)
{
	if (!wchar)
	{
		if (flag->width > 0)
			flag->width--;
		prt->spec = ft_strdup("\0");
		prt->returnvalue++;
		ft_putchar(0);
	}
	else
	{
		prt->spec = ft_strnew(ft_wbitslen(&wchar));
		ft_strwchar(wchar, prt->spec, 0);
	}
}

void		wchar_to_char(wchar_t *wchar, t_prt *prt, t_flag *flag)
{
	int i;
	int n;

	n = 0;
	i = 0;
	if (!wchar)
		prt->spec = ft_strdup("(null)");
	else
	{
		prt->spec = ft_strnew(ft_wbitslen(wchar));
		while (wchar[i])
		{
			if ((n < flag->precision && ft_wcharlen(wchar[i])
						+ n <= flag->precision) || (flag->precision <= 0))
				n = ft_strwchar(wchar[i], prt->spec, n);
			i++;
		}
	}
}
