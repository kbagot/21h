/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 18:45:13 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/11 11:48:19 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		f_l(char const *str, int i, char *c)
{
	int lettre;

	lettre = 0;
	while (str[i] && ft_strchr(c, str[i]) == NULL)
	{
		i++;
		lettre++;
	}
	return (lettre);
}

static int		f_m(char const *str, char *c)
{
	int i;
	int mot;

	mot = 0;
	i = 0;
	while (str[i] && ft_strchr(c, str[i]))
		i++;
	while (str[i])
	{
		while (str[i] && ft_strchr(c, str[i]) == NULL)
			i++;
		mot++;
		i++;
	}
	if (ft_strchr(c, str[ft_strlen(str)]))
		mot++;
	return (mot);
}

char			**strmsplit(char const *s, char *c)
{
	int		i;
	int		j;
	int		k;
	char	**split;

	i = 0;
	j = 0;
	if (s == NULL || (split = malloc(sizeof(char*) * (f_m(s, c) + 1))) == NULL)
		return (NULL);
	while (s[i])
	{
		while (s[i] && ft_strchr(c, s[i]))
			i++;
		if (s[i])
		{
			k = 0;
			if ((split[j] = malloc(sizeof(char) * (f_l(s, i, c) + 1))) == NULL)
				return (NULL);
			while (s[i] && ft_strchr(c, s[i]) == NULL)
				split[j][k++] = s[i++];
			split[j++][k++] = '\0';
		}
	}
	split[j] = NULL;
	return (split);
}
