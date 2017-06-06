/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/27 14:40:50 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/06 19:51:16 by kbagot           ###   ########.fr       */
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
}					t_data;

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
int					exec_echo(char *stin);
char				*unsplit(char **dt);
t_env				*env_build(char **env, t_env *first_env);
void				swap_dat(char **dat, int i);
char				*join(char *s1, char *s2, char *s3);
void				destroy_env(t_env **env);
void				addtmp_env(char **cstin, char *add, t_env **tmp_env,
		int del);
int					exec_cd(char **cstin, t_env *env);
char				*line_edit(t_data *data);
void				init_term(void);
void				reset_term(void);
void				arrow_key(t_data *data, char **stin, char *buff);
int					print(int c);
void				copy_cut(t_data *data, char **stin, char *buff);
void				writer(t_data *data, char **stin, char *buff);
void				move_by_word(t_data *data, char *stin, char *buff);
char				**strquotesplit(char const *s, char *c);
char				**splitforquote(char const *s, char *c);
void				act_pos(t_data *d);
void				go_home(t_data *data);
void				go_end(t_data *data, char *stin);
void				move_left(t_data *data);
void				move_right(t_data *data);
void				reset_line(t_data *data, char *stin);
void				cleaner(t_data *data);

#endif
