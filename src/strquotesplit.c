/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 18:35:32 by kbagot            #+#    #+#             */
/*   Updated: 2017/05/23 19:43:26 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		f_l(char const *str, int i, char *c)
{
	int lettre;
	int quote;

	quote = 0;
	lettre = 0;
	while (str[i] && (quote != 0 || ft_strchr(c, str[i]) == NULL))
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
		lettre++;
	}
	return (lettre);
}

static int		f_m(char const *str, char *c)
{
	int i;
	int mot;
	int quote;

	quote = 0;
	mot = 0;
	i = 0;
	while (str[i])
	{
//	ft_printf("%s\n", &str[i]);
		while (str[i] && ft_strchr(c, str[i]))
			i++;
		if (str[i])
		{
			while (str[i] && (quote != 0 || ft_strchr(c, str[i]) == NULL))
			{
				if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
					quote = str[i];
				else if (str[i] == quote)
					quote = 0;
				i++;
			}
		mot++;
		}
	}
	return (mot);
}

char			**strquotesplit(char const *s, char *c)
{
	int		i;
	int		j;
	int		k;
	char	**split;
	int		quote;

	quote = 0;
	i = 0;
	j = 0;
//	ft_printf("s\n");
	if (s == NULL || (split = malloc(sizeof(char*) * (f_m(s, c) + 1))) == NULL)
		return (NULL);
//	ft_printf("s1  %d\n", f_m(s, c));
	while (s[i])
	{
		while (s[i] && ft_strchr(c, s[i]))
			i++;
		if (s[i])
		{
			k = 0;
			quote = 0;
			if ((split[j] = malloc(sizeof(char) * (f_l(s, i, c) + 1))) == NULL)
				return (NULL);
			while (s[i] && (quote != 0 || ft_strchr(c, s[i]) == NULL))
			{
				if ((s[i] == '\'' || s[i] == '\"') && quote == 0)
					quote = s[i];
				else if (s[i] == quote)
					quote = 0;
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
	ft_printf("SALUT\n");
	char **lol;if (ac == 2){	lol = strquotesplit(av[1], ";");if(lol)while(*lol){printf("[%s]\n", *lol);lol++;}}return (0);
}*/
