/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 18:45:13 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/15 20:33:47 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		f_l(char const *str, int i, char c)
{
	int lettre;

	lettre = 0;
	while (str[i] && str[i] != c)
	{
		i++;
		lettre++;
	}
	return (lettre);
}

static int		f_m(char const *str, char c)
{
	int i;
	int mot;

	mot = 0;
	i = 0;
	while (str[i] && str[i] == c)
		i++;
	while (str[i])
	{
		while (str[i] && str[i] != c)
			i++;
		mot++;
		i++;
	}
	if (str[ft_strlen(str)] == c)
		mot++;
	return (mot);
}

char			**ft_strsplit(char const *s, char c)
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
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			k = 0;
			if ((split[j] = malloc(sizeof(char) * (f_l(s, i, c) + 1))) == NULL)
				return (NULL);
			while (s[i] && s[i] != c)
				split[j][k++] = s[i++];
			split[j++][k++] = '\0';
		}
	}
	split[j] = NULL;
	return (split);
}
