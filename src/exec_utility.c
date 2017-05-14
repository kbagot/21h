/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:58:40 by kbagot            #+#    #+#             */
/*   Updated: 2017/05/14 21:06:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	invalid_exec(char **stin, char **env, t_data *data)
{
	if ((stin[0] && (access(stin[0], X_OK) == -1)) || !stin[0])
	{
		ft_tabdel(&env);
		if (stin[0])
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(stin[0], 2);
			ft_putchar_fd('\n', 2);
			data->rvalue = 1;
		}
		return (1);
	}
	return (0);
}

static void	sig(int rvalue, pid_t pid, char *stin)
{
	if (WTERMSIG(rvalue) == SIGSEGV)
	{
		ft_putnbr_fd(pid, 2);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd("segmentation fault", 2);
		ft_putstr_fd("  ", 2);
		ft_putstr_fd(stin, 2);
		ft_putchar_fd('\n', 2);
	}
	else if (WTERMSIG(rvalue) == SIGBUS)
	{
		ft_putnbr_fd(pid, 2);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd("bus error", 2);
		ft_putstr_fd("  ", 2);
		ft_putstr_fd(stin, 2);
		ft_putchar_fd('\n', 2);
	}
}
static void	exec_utility(char **env, char **stin, t_data *data)
{
	pid_t	pid;
	int		rvalue;

	rvalue = 0;
	if (invalid_exec(stin, env, data))
		return ;
	if ((pid = fork()) == 0)
	{
		if (execve(stin[0], stin, env) == -1)
		{
			ft_putstr_fd("Exec format error\n", 2);
			_exit(1);
		}
	}
	else if (pid > 0)
	{
		wait(&rvalue);
		if (WIFEXITED(rvalue))
			data->rvalue = WEXITSTATUS(rvalue);
		else if (WIFSIGNALED(rvalue))
			sig(rvalue, pid, stin[0]);
	}
	ft_tabdel(&env);
}

static void		find_pipe(char **cstin, int *j, int *k)
{
	int i;

	i = 0;
	while (cstin[i] && ft_strcmp(cstin[i], "|") != 0)
	{
		if (ft_strchr(cstin[i], '>') || ft_strchr(cstin[i], '<'))
			*k += 1;
		else
			*j += 1;
		i++;
	}
	//	printf("[%d]  [%d]\n", *j, *k);
}

static t_line	*redir_operator(char **cstin)
{
	t_line *line;
	int		i;
	int		j;
	int		k;
	t_line *save;

	save = NULL;
	i = 0;
	j = 0;
	k = 0;
	while (cstin[i])
	{
		if (save == NULL)
		{
			line = ft_memalloc(sizeof(t_line));
			save = line;
		}
		find_pipe(&cstin[i], &j, &k);
		line->proc = (char**)malloc(sizeof(char*) * j + 1);
		line->redirect = (char**)malloc(sizeof(char*) * k + 1);
		j = 0;
		k = 0;
		while (cstin[i] && (ft_strcmp(cstin[i], "|") != 0))
		{//add redirect cmd
			char *c;
			if ((c = ft_strchr(cstin[i], '>')) || (c = ft_strchr(cstin[i], '<')))
			{
				if (c == cstin[i])
				{
	line->redirect[k++] = join(ft_strdup(cstin[i]), " ", cstin[i + 1]);
				}
				else
				{
					int l = c - cstin[i] - 1;
		//			printf("%d\n", l);
		//			printf("[%s]\n", cstin[i]);
					while (l >= 0)
					{
						if (!ft_isdigit(cstin[i][l]) && cstin[i][l] != '&')
						{
							line->proc[j++] = ft_strsub(cstin[i],
									0, c - cstin[i]);
							line->redirect[k++] = 
							join(ft_strdup(&cstin[i][c - cstin[i]]), " ",
									cstin[i + 1]);
//							printf("[%s] [%s]\n", line->proc[j - 1],
//									line->redirect[k - 1]);
							break ;
						}
						else
						{
						char *tmp;
						char *tmp2;
tmp = ft_strsub(cstin[i], 0, c - cstin[i]);
tmp2 = ft_strjoin(c, " ");
line->redirect[k++] = join(ft_strjoin(tmp, " "), tmp2, cstin[i + 1]);
ft_strdel(&tmp);
ft_strdel(&tmp2);
		//			printf("[%s]\n", line->redirect[k-1]);
							break ;}
						l--;
					}
				}
				i++;
			}
			else
				line->proc[j++] = ft_strdup(cstin[i]);
			if (!cstin[i])
				break ;
			i++;
		}
		line->redirect[k] = NULL;
		line->proc[j] = NULL;
		line->next = NULL;
		if (!cstin[i])
			break ;
		line->next = ft_memalloc(sizeof(t_line));
		line = line->next;
		i++;
	}
	return (save);
}

static t_line	*fork_pipes(t_line *line, t_data *d)
{
	t_line *old;
	pid_t	pid;
	int		fd[2];

	pid = 0;
	if (line)
	{
		while (line->next)
		{
			pipe(fd);
			d->out = fd[1];
			if ((pid = fork ()) == 0)
			{
				if (d->in != 0)
				{
					dup2 (d->in, 0);
					close (d->in);
				}
				if (d->out != 1)
				{
					dup2 (d->out, 1);
					close (d->out);
				}
				return (line);
			}
			close (d->out);
			d->in = fd[0];
			old = line;
			line = line->next;
			ft_tabdel(&old->proc);
			ft_tabdel(&old->redirect);
			free(old);
		}
		if (d->in != 0)
			dup2(d->in, 0);
	}
	return (line);
}

static void exec_redir(char **rdr, t_data *d)
{
	int i;
	int j;
	int up;
	char **cmd;

	j = 0;
	i = 0;
	while (rdr[j])
	{
		cmd = ft_strsplit(rdr[j], ' ');
		while (cmd[i])
			i++;
		i--;
		if (i >= 0 && !ft_strcmp(cmd[i - 1], ">"))
		{
			if (i != 1)
				up = ft_atoi(cmd[0]);
			else
				up = 1;
			d->out = open(cmd[i], O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(d->out, up);
			close (d->out);
		}
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">>"))
		{
			if (i != 1)
				up = ft_atoi(cmd[0]);
			else
				up = 1;
			d->out = open(cmd[i], O_CREAT | O_WRONLY | O_APPEND,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(d->out, up);
			close (d->out);
		}
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">&"))
		{
			if (i != 1)
				up = ft_atoi(cmd[0]);
			else
				up = 1;
			d->out = open(cmd[i], O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			dup2(d->out, up);
			close (d->out);
		}
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<"))
		{
			if (i != 1)
				up = ft_atoi(cmd[0]);
			else
				up = 0;
			d->in = open(cmd[i], O_RDONLY);
			dup2(d->in, up);
			close (d->in);
		}
		i = 0;
	j++;
	}
}

void		parse_entry(t_env **s_env, char **cstin, char *stin, t_data *data)
{
	t_env	*tmp_env;
	t_line	*line;

	data->stdin_cpy = dup(0);
	data->stdout_cpy = dup(1);
	data->in = 0;
	data->out = 1;
	line = redir_operator(cstin);
	line = fork_pipes(line, data);
	exec_redir(line->redirect, data);
	cstin = line->proc;
	tmp_env = NULL;
	if ((data->rvalue = builtin(cstin, s_env, stin, data)))
	{
		if (line->next)
			exit(data->rvalue);
		else
			return ;// exit if pipe
	}
	tmp_env = master_env(*s_env, cstin, tmp_env);
	if ((cstin = utility(cstin, *s_env)))
		exec_utility(list_to_tab(tmp_env), cstin, data);
	destroy_env(&tmp_env);
	if (line->next)
	{
		_exit(data->rvalue);
	}
	ft_tabdel(&line->proc);
	ft_tabdel(&line->redirect);
	free(line);

	int status;
	int pid;
	while ((pid = wait(&status)) > 0)
	{
		status = 0;;
}
	if (data->out != 1)
	{
		close(data->out);
		dup2(data->stdout_cpy, 1);
		close(data->stdout_cpy);
	}
	if (data->in != 0)
	{
		close(data->in);
		dup2(data->stdin_cpy, 0);
		close(data->stdin_cpy);
	}
} 
