/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_uni.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 18:16:41 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:59 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_wcharlen(wchar_t wchar)
{
	if (wchar <= 0x7f)
		return (1);
	else if (wchar <= 0x7ff)
		return (2);
	else if (wchar <= 0xffff)
		return (3);
	else if (wchar <= 0x10ffff)
		return (4);
	return (0);
}

size_t		ft_wbitslen(wchar_t *wstr)
{
	size_t	len;
	size_t	bits;

	len = ft_wstrlen(wstr);
	bits = 0;
	while (len-- > 0)
		bits = bits + ft_wcharlen(*wstr++);
	return (bits);
}

size_t		ft_wstrlen(wchar_t *wstr)
{
	size_t	len;

	len = 0;
	if (!wstr)
		return (0);
	while (*(wstr++))
		len++;
	return (len);
}
