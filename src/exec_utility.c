/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:58:40 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/14 18:44:22 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern int g_signo;

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
		g_signo = pid;
		wait(&rvalue);
		if (data->lastpid != 0)
			kill(data->lastpid, SIGTERM);
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
		if (!ft_strchr("\"\'", cstin[i][0] && (ft_strchr(cstin[i], '>') ||
						ft_strchr(cstin[i], '<'))))
			*k += 1;
		else
			*j += 1;
		i++;
	}
}

static void		stock_redir_fd(t_line *line, char **cstin, int *k, char *c)
{
	char *tmp;
	char *tmp2;

	tmp = ft_strsub(cstin[0], 0, c - cstin[0]);
	tmp2 = ft_strjoin(c, " ");
	line->redirect[*k] = join(ft_strjoin(tmp, " "), tmp2, cstin[1]);
	ft_strdel(&tmp);
	ft_strdel(&tmp2);
	*k += 1;
}

static void		stock_redir(char **cstin, t_line *line, int *j, int *k)
{
	char *c;

	c = NULL;
	if (!(c = ft_strchr(*cstin, '>')))
		c = ft_strchr(*cstin, '<');
	if (c == *cstin)
	{
		line->redirect[*k] = join(ft_strdup(*cstin), " ", cstin[1]);
		*k += 1;
	}
	else
	{
		if (!ft_isnbr(ft_strsub(*cstin, 0, c - *cstin)))
		{
			line->proc[*j] = ft_strsub(*cstin, 0, c - *cstin);
			line->redirect[*k] = join(ft_strdup(&cstin[0][c - *cstin]), " ",
					cstin[1]);
			*j += 1;
			*k += 1;
		}
		else
			stock_redir_fd(line, cstin, k, c);
	}
}

static void		separate_line(char **cstin, int *i, t_line *line)
{
	int j;
	int k;
	char *c;

	k = 0;
	j = 0;
	while (cstin[*i] && (ft_strcmp(cstin[*i], "|") != 0))
	{//add redirect cmd
		if (!ft_strchr("\'\"", cstin[*i][0]) &&
				((c = ft_strchr(cstin[*i], '>')) || (c = ft_strchr(cstin[*i], '<'))))
		{
			stock_redir(&cstin[*i], line, &j, &k);
			*i += 1;
		}
		else
		{
			if (ft_strchr("\'\"", cstin[*i][0]))
				line->proc[j++] = ft_strdup(&cstin[*i][1]);
			else
				line->proc[j++] = ft_strdup(cstin[*i]);
		}
		if (!cstin[*i])
			break ;
		*i += 1;
	}
	line->redirect[k] = NULL;
	line->proc[j] = NULL;
}

static t_line	*split_pipe(char **cstin)
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
		line->proc = (char**)malloc(sizeof(char*) * (j + 1));
		line->redirect = (char**)malloc(sizeof(char*) * (k + 1));
		separate_line(cstin, &i, line);
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
			d->lastpid = pid;
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


static void	redir_output(char **cmd, t_data *d, int i)
{//  >
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 1;
	if ((d->out = open(cmd[i], O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) != -1)
		dup2(d->out, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->out >= 0 && d->out <= 2))
		close (d->out);
}

static void	redir_input(char **cmd, t_data *d, int i)
{// <
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 0;
	if ((d->in = open(cmd[i], O_RDONLY)) != -1)
		dup2(d->in, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->in >= 0 && d->in <= 2))
		close (d->in);
}

static void	append_redir_output(char **cmd, t_data *d, int i)
{// >>
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 1;
	if ((d->out = open(cmd[i], O_CREAT | O_WRONLY | O_APPEND,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) != -1)
		dup2(d->out, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->in >= 0 && d->in <= 2))
		close (d->out);
}

static int	dup_output(char **cmd, int i)
{
	int n;
	int w;

	//printf("[%s][%s][%s] %d \n", cmd[0], cmd[1], cmd[2], i);
	if (i != STDOUT_FILENO)
		n = ft_atoi(cmd[0]);
	else
		n = STDOUT_FILENO;
	if (!ft_strcmp(cmd[i], "-"))
		close (n);
	if (ft_isnbr(cmd[i]))
	{
		int t;
		t = -1;
		//ft_printf("%d\n", d->out);
		w = ft_atoi(cmd[i]);
		//		printf("%d \n", d->out);
		if ((w >= 10 && w <= 12) || (t = dup(w)) == -1)
		{
			//		printf("%d \n", t);
			//		printf("%d \n", w);
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(w, 2);
			ft_putstr_fd(": Bad file descriptor\n", 2);
			return (1);
		}
		if (t != -1)
		{
			//		printf("%ddelt \n", t);
			close (t);
		}
		dup2(w, n);
	}
	return (0);
}

static int	dup_input(char **cmd, int i)
{ //mirror of upper fct 
	int n;
	int w;

	//printf("[%s][%s][%s] %d \n", cmd[0], cmd[1], cmd[2], i);
	if (i != STDIN_FILENO)
		n = ft_atoi(cmd[0]);
	else
		n = STDIN_FILENO;
	if (!ft_strcmp(cmd[i], "-"))
		close (n);
	if (ft_isnbr(cmd[i]))
	{
		int t;
		t = -1;
		//ft_printf("%d\n", d->out);
		w = ft_atoi(cmd[i]);
		//		printf("%d \n", d->out);
		if ((w >= 10 && w <= 12) || (t = dup(w)) == -1)
		{
			//		printf("%d \n", t);
			//		printf("%d \n", w);
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(w, 2);
			ft_putstr_fd(": Bad file descriptor\n", 2);
			return (1);
		}
		if (t != -1)
		{
			//		printf("%ddelt \n", t);
			close (t);
		}
		dup2(w, n);
	}
	return (0);
}

static int exec_redir(char **rdr, t_data *d)
{
	int i;
	int j;
	char **cmd;

	j = 0;
	i = 0;
	cmd = NULL;
	while (rdr[j])
	{
		cmd = ft_strsplit(rdr[j], ' ');
		while (cmd[i])
			i++;
		i--;
		if (i >= 0 && !ft_strcmp(cmd[i - 1], ">"))
			redir_output(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<"))
			redir_input(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">>"))
			append_redir_output(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">&"))
		{
			if (dup_output(cmd, i))
				return (1);
		}
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<&"))
		{
			if (dup_input(cmd, i))
				return (1);
		}
		i = 0;
		j++;
	}
	ft_tabdel(&cmd);
	return (0);
}

int		get_proc(int sign)
{
	static int val = 2;

	if (sign != 0)
		val = sign;
	return (val);
}


void kill_procs(int sig)
{
	if (sig == SIGINT)
	{
		if (g_signo > 0)
		{
			kill(g_signo, 9);
			ft_putchar('\n');
		}
	}
}

static void reset_fd(t_data *data)
{
	dup2(data->stdin_cpy, 0);
	close(data->stdin_cpy);
	dup2(data->stdout_cpy, 1);
	close(data->stdout_cpy);
	dup2(data->stderr_cpy, 2);
	close(data->stderr_cpy);
	//	ft_printf("%d %d %d \n", data->in , data->out, data->err); Should keep it ?
	//	if (data->in != 0)
	//		close (data->in);
	//	if (data->out != 1)
	//		close (data->out);
	//	if (data->err != 2)
	//		close (data->err);
}

static void	save_fd(t_data *data)
{
	data->stdin_cpy = 10;
	data->stdout_cpy = 11;
	data->stderr_cpy = 12;
	dup2(STDIN_FILENO, 10);
	dup2(STDOUT_FILENO, 11);
	dup2(STDERR_FILENO, 12);
	data->in = STDIN_FILENO;
	data->out = STDOUT_FILENO;
	data->err = STDERR_FILENO;
}

static int enter_builtin(t_data *data, t_env **s_env, char *stin, t_line *line)
{
	if ((data->rvalue = exec_redir(line->redirect, data)) ||
			(data->rvalue = builtin(line->proc, s_env, stin, data)))
	{
		if (line->next)
		{
			_exit(data->rvalue);
		}
		else
		{
			reset_fd(data);
			return (1);
		}
	}
	return (0);
}

static void	enter_utility(t_data *data, t_env **s_env, t_line *line)
{
	t_env	*tmp_env;

	tmp_env = NULL;
	tmp_env = master_env(*s_env, line->proc, tmp_env);
	if ((line->proc = utility(line->proc, *s_env)))
		exec_utility(list_to_tab(tmp_env), line->proc, data);
	if (line->next)
		_exit(data->rvalue);
	destroy_env(&tmp_env);
	ft_tabdel(&line->proc);
	ft_tabdel(&line->redirect);
	free(line);
}

void		parse_entry(t_env **s_env, char **cstin, char *stin, t_data *data)
{
	t_line	*line;

	save_fd(data);
	line = split_pipe(cstin);
	line = fork_pipes(line, data);
	if (!enter_builtin(data, s_env, stin, line))
		enter_utility(data, s_env, line);
	else
		return ;
	int status;
	int pid;
	while ((pid = wait(&status)) > 0)
		status = 0;
	reset_fd(data);
}
