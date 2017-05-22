/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 14:22:38 by kbagot            #+#    #+#             */
/*   Updated: 2017/05/22 20:07:01 by kbagot           ###   ########.fr       */
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
		{	i++;
		letter++;}
		i++;
	}
	else
		while (str[i] && (ft_strchr(c, str[i]) == NULL))
		{i++;letter++;
			if (str[i] == '\'' || str[i] == '\"')
				break ;}
	return (letter);
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
	//	printf("%c   %d\n", str[i], i);
		while (str[i] && ft_strchr(c, str[i]))
			i++;
		quote = 0;
		if (str[i] && (str[i] == '\"' || str[i] == '\''))
		{
		//printf("**%s   \n", &str[i]);
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			i++;
		mot++;
		}
		else if (str[i])
		{
	//	printf("$$%s   \n", &str[i]);
			while (str[i] && (ft_strchr(c, str[i]) == NULL))
			{	i++;
			if (str[i] == '\'' || str[i] == '\"')
				break ;}
		mot++;}
	}
//	if (ft_strchr(c, str[ft_strlen(str)]))
//		mot++;
//		printf("WTF[%d]\n", mot);
	return (mot);
}

char			**splitforquote(char const *s, char *c)
{
	int		i;
	int		j;
	int		k;
	char	**split;
	int		quote;

	i = 0;
	j = 0;
	//	printf("s\n");
	if (s == NULL || (split = malloc(sizeof(char*) * (f_m(s, c) + 1))) == NULL)
		return (NULL);
	//	printf("s1\n");
	while (s[i])
	{
		k = 0;
		while (s[i] && ft_strchr(c, s[i]))
			i++;
	//	printf("s2\n");
		if (s[i])
		{
			if ((split[j] = malloc(sizeof(char) * (f_l(s, i, c) + 1))) == NULL)
				return (NULL);
	//	printf("s3\n");
			quote = 0;
			if (s[i] == '\"' || s[i] == '\'')
			{
				quote = s[i];
		//printf("s4\n");
				split[j][k++] = s[i++];
				while (s[i] && s[i] != quote)
					split[j][k++] = s[i++];
				i++;
		//printf("s5\n");
			}
			else
				while (s[i] && ft_strchr(c, s[i]) == NULL)
				{if (s[i] == '\'' || s[i] == '\"')
				break ;
				split[j][k++] = s[i++];}
	//	printf("s6\n");
			split[j++][k++] = '\0';
			printf("%s\n", split[j - 1]);
		}
	//	printf("s7\n");
	}
	split[j] = NULL;
return (split);
}
/*
int main(int ac, char **av)
{
	char **lol;if (ac == 2){	lol = strquotesplit(av[1], ";");if(lol)while(*lol){printf("[%s]\n", *lol);lol++;}}return (0);
}*/
