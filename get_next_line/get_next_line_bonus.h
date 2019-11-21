/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:30:55 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/21 19:19:23 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

typedef struct		s_fd
{
	int				fd;
	char			*rest;
	int				len_rest;
	int				len_line;
	int				eof;
	struct s_fd		*next;
}					t_fd;

# define ADD 1
# define REMOVE 0

# define END_LINE_CHAR '\n'

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

t_fd				*new_link(int fd);
t_fd				*manage_link(int fd, t_fd	**list, int add);
int					get_next_line(int fd, char **line);
int					to_read_or_not_to_read(t_fd *link, char **line);
int					update_strings(char **line, char *parse_me, t_fd *link, \
		int previous);
int					update_rest(char *str, t_fd *link, int start, int old_len);
int					update_line(char **line, char *str2, t_fd *link, \
		int *found);

#endif
