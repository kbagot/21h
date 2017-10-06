/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/27 14:40:50 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/06 16:12:41 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H

# include "../libft/includes/libft.h"
# include <paths.h>
# include <limits.h>
# include <sys/stat.h>
# include <curses.h>// care order
# include <term.h>//
# include <termcap.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>

typedef struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct		s_data
{
	int				line_count;
	int				start_row;
	int				start_col;
	int				row;
	int				col;
	int				scr_row;
	int				scr_col;
	int				rvalue;
	struct s_hist	*hist;
	int				cursor;
	char			*clipboard;
	struct s_line	*line;
	int 			in;
	int 			out;
	int				err;
	int				stdin_cpy;
	int				stdout_cpy;
	int				stderr_cpy;
	int				lastpid;
	struct termios	stored_settings;
	struct s_ansi	*a;
}					t_data;

typedef struct		s_ansi
{
	char			*cm_s;
	char			*dl_s;
	char			*do_s;
	char			*le_s;
	char			*nd_s;
	char			*sc_s;
	char			*rc_s;
	char			*ku_s;
	char			*kd_s;
	char			*kr_s;
	char			*kl_s;
	char			*cl_s;
	char			*up_s;
	char			*dc_s;
	char			*se_s;
	char			*so_s;
}					t_ansi;

typedef struct		s_line // line piped
{
	char			**proc;
	char			**redirect;
	struct s_line	*next;
}					t_line;

typedef struct		s_hist
{
	char			*elem;
	struct s_hist	*next;
	struct s_hist	*before;
}					t_hist;

typedef struct		s_edit
{
	char	*buff;
	char	*stin;
	int		ler;
}					t_edit;

typedef struct		s_split
{
	char const		*s;
	char			*c;
	int				i;
	int				j;
	int				k;
	char			**split;
	int				quote;
}					t_split;

void				parse_entry(t_env **s_env, char **cstin, char *stin,
		t_data *data);
t_env				*search_env(t_env *env, char *name);
char				**list_to_tab(t_env *lenv);
int					builtin(char **cstin, t_env **env, char *stin,
		t_data *data);
char				**utility(char **st, t_env *s_env);
t_env				*master_env(t_env *s_env, char **cstin, t_env *tmp_env);
void				delete_env(t_env **env, char *name);
void				add_env(t_env *env, char **cstin);
int					exec_echo(char **stin);
char				*unsplit(char **dt);
t_env				*env_build(char **env, t_env *first_env);
void				swap_dat(char **dat, int i);
char				*join(char *s1, char *s2, char *s3);
void				destroy_env(t_env **env);
void				addtmp_env(char **cstin, char *add, t_env **tmp_env,
		int del);
int					exec_cd(char **cstin, t_env *env);
char				*line_edit(t_data *data);
void				init_term(t_data *d);
void				reset_term(t_data *d);
void				arrow_key(t_data *data, char **stin, char *buff);
int					print(int c);
void				copy_cut(t_data *data, char **stin, char *buff);
void				writer(t_data *data, char **stin, char *buff);
void				move_by_word_line(t_data *data, char *stin, char *buff);
char				**strquotesplit(char const *s, char *c);
char				**splitforquote(char const *s, char *c);
void				act_pos(t_data *d);
void				go_home(t_data *data);
void				go_end(t_data *data, char *stin);
void				move_left(t_data *data);
void				move_right(t_data *data);
void				reset_line(t_data *data, char *stin);
void				cleaner(t_data *data);
void				kill_procs(int sig);
int					get_proc(int sign);
int					move_cursor(t_data *data, char *stin, char *buff);
void				save_fd(t_data *data);
void				show_prompt(t_env *s_env, t_data *data);
void				creat_heredoc(t_data *d, char **s);
int					parse_error(char **stin);
void				init_l_edit(t_edit *e, t_data *data);
char				*end_line(t_data *data, t_edit *e);
int					conform_quote(char *s);
char				*conform(char *stin);
void				paste(t_data *data, char **stin);
char				*delete_one(char *stin, t_data *data);
void				add_history(char *cmd, t_data *data);
int					f_l(char const *str, int i, char *c);
int					f_m(char const *str, char *c);
int					exec_redir(char **rdr, t_data *d);
t_line				*split_pipe(char **cstin);
int					exec_exit(char **cstin, t_data *data, char *stin);
void				exec_utility(char **env, char **stin, t_data *data);
t_line				*fork_pipes(t_line *line, t_data *d);

#endif
