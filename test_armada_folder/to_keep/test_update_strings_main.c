/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_update_strings_main.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 17:14:39 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/16 18:17:40 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

void	fd_links_print(fd_gnl *list)
{
	int link;

	link = 0;
	printf("in link: %d, fd is:%d\n", link, list->fd);
	if (list->rest)
		printf("link->rest = \"%s\"\n", list->rest);
	else
		printf("link->rest is a NULL pointer\n");
	printf("len_rest is: %d\n", list->len_rest);
	printf("len_line is: %d\n", list->len_line);
	printf("eof is :%d\n", list->eof);
	printf("\n");
}

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	len;
	char	*ptr;

	i = 0;
	len = ft_strlen(s1);
	if (!(ptr = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	while (i < len)
	{
		*(ptr + i) = *(s1 + i);
		i++;
	}
	*(ptr + i) = '\0';
	return (ptr);
}

/*
** RETURN: new link matching the file descriptor.
*/

fd_gnl	*new_link(int fd)
{
	fd_gnl	*new_link;

	if (!(new_link = (fd_gnl*)malloc(sizeof(*new_link))))
		return (NULL);
	new_link->fd = fd;
	new_link->rest = NULL;
	new_link->len_rest = 0;
	new_link->len_line = 0;
	new_link->eof = 0;
	new_link->next = NULL;
	return (new_link);
}


int		update_strings(char **line, char *parse_me, int parse_me_len, fd_gnl *link)
{
	int start_rest;
	int found;

	found = 0;
	if ((start_rest = update_line(line, parse_me, link)) == -1)
		return (-1);
	printf("start_rest : %d\n", start_rest);
	if (parse_me[start_rest] == '\n')
	{
		start_rest++;
		found = 1;
	}
 	if (!(update_rest(parse_me, link, start_rest)))
		return (-1);
	if (!found || (found && !parse_me[start_rest] && !link->eof))
		return (1);
	return (0);
}

int	update_rest(char *str, fd_gnl *link, int start)
{
	int		i;
	char	*tmp;
	int		new_rest;

	tmp = link->rest;
	i = 0;
	new_rest = 0;
	while (str[start + i])
		i++;
	if (!(link->rest = (char*)malloc(sizeof(char) * (i + 1))))
		return (0);
	link->rest[i] = '\0';
	while (new_rest < i)
	{
		link->rest[new_rest] = str[start + new_rest];
		new_rest++;
	}
	(tmp != NULL) ? free(tmp) : 0;
	link->len_rest = new_rest;
	return (1);
}

int	update_line(char **line, char *str2, fd_gnl *link)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = *line;
	i = 0;
	j = -1;
	while (str2[i] && (str2[i] != '\n'))
		i++;
	if (!(*line = (char*)malloc(sizeof(char) * (link->len_line + i + 1))))
		return (-1);
	(*line)[link->len_line + i] = '\0';
	while (++j < link->len_line + i)
	{
		if (j < link->len_line)
			(*line)[j] = tmp[j];
		else
			(*line)[j] = str2[j - link->len_line];
	}
	link->len_line = link->len_line + i;
	if (tmp)
		free(tmp);
	return (i);
}

int	main()
{
	int		fd1;
	char	*line;
	static fd_gnl	*link;

	line = NULL;
	if((fd1 = open("./file_lines2", O_RDONLY)) == -1)
	{
		printf("failed to oPen the fd\n");
		return (0);
	}
	link = new_link(fd1);
	link->rest = ft_strdup("salut!");
	//line = ft_strdup("hello 789d");
	//link->len_line = 10;

	update_strings(&line, "pa\n de bac", 10, link);
	fd_links_print(link);
	printf("\nline: \"%s\"\n\n", line);

	update_strings(&line, link->rest, link->len_rest, link);
	fd_links_print(link);
	printf("\nline: \"%s\"\n\n", line);

	update_strings(&line, link->rest, link->len_rest, link);
	fd_links_print(link);
	printf("\nline: \"%s\"\n", line);
	if (line)
		free(line);
	close(fd1);
	return (0);
}
