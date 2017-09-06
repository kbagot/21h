/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 14:22:38 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/06 19:51:40 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		f_l(char const *str, int i, char *c)
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

static void		f_mnorm(int *quote, int *i, int *mot, char const *str)
{
	*quote = str[*i];
	*i += 1;
	while (str[*i] && str[*i] != *quote)
		*i += 1;
	*i += 1;
	*mot += 1;
}

static int		f_m(char const *str, char *c)
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

static int  stock_q(int *i, int *j, char **split, char const *s)
{
	int		quote;
	int		k;

	k = 0;
	quote = 0;
	quote = s[*i];
	if (s[*i] == '\"' || s[*i] == '\'')
	{
		split[*j][k++] = s[*i];
		*i += 1;
		while (s[*i] && s[*i] != quote)
		{
			split[*j][k++] = s[*i];
			*i += 1;
		}
		*i += 1;
		return (k);
	}
	return (0);
}

char			**splitforquote(char const *s, char *c)
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
		k = 0;
		while (s[i] && ft_strchr(c, s[i]))
			i++;
		if (s[i])
		{
			if ((split[j] = malloc(sizeof(char) * (f_l(s, i, c) + 1))) == NULL)
				return (NULL);
			if (!(k = stock_q(&i, &j, split, s)))
				while (s[i] && ft_strchr(c, s[i]) == NULL)
				{
					if (s[i] == '\'' || s[i] == '\"')
						break ;
					split[j][k++] = s[i++];
				}
			split[j++][k++] = '\0';
		}
	}
	split[j] = NULL;
	return (split);
}
/*
   int main(int ac, char **av)
   {
   char **lol;if (ac == 2){	lol = strquotesplit(av[1], ";");if(lol)while(*lol){printf("[%s]\n", *lol);lol++;}}return (0);
   }*/
