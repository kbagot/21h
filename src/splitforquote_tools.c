/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitforquote_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 20:31:32 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/06 22:18:41 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			f_l(char const *str, int i, char *c)
{
	int letter;
	int quote;

	letter = 0;
	quote = 0;
	if (str[i] == '\"' || str[i] == '\'')
	{
		quote = str[i];
		i++;
		while (str[i] && str[i] != quote)
		{
			i++;
			letter++;
		}
		i++;
	}
	else
		while (str[i] && (ft_strchr(c, str[i]) == NULL))
		{
			i++;
			letter++;
			if (str[i] == '\'' || str[i] == '\"')
				break ;
		}
	return (letter);
}

static void	f_mnorm(int *quote, int *i, int *mot, char const *str)
{
	*quote = str[*i];
	*i += 1;
	while (str[*i] && str[*i] != *quote)
		*i += 1;
	*i += 1;
	*mot += 1;
}

int			f_m(char const *str, char *c)
{
	int i;
	int mot;
	int	quote;

	mot = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(c, str[i]))
			i++;
		quote = 0;
		if (str[i] && (str[i] == '\"' || str[i] == '\''))
			f_mnorm(&quote, &i, &mot, str);
		else if (str[i])
		{
			while (str[i] && (ft_strchr(c, str[i]) == NULL))
			{
				i++;
				if (str[i] == '\'' || str[i] == '\"')
					break ;
			}
			mot++;
		}
	}
	return (mot);
}
