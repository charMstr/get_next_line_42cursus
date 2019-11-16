/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:28:41 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/16 01:02:15 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

typedef struct		s_fd_gnl
{
	int				fd;
	char			*rest;
	int				len_rest;
	int				len_line;
	int				eof;
	struct s_fd_gnl	*next;
}					fd_gnl;

# define ADD 1
# define REMOVE 0

fd_gnl	*manage_link(int fd, fd_gnl	**list, int add);
fd_gnl	*new_link(int fd);
int		get_next_line(int fd, char **line);
int		to_read_or_not_to_read(fd_gnl *fd_link, char **line);

#endif
