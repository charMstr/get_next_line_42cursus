/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_GNL_FINISHED_multifd_main.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:46:43 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/17 20:10:24 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

int	main(int argc, char *argv[])
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line;
	int		result;

	line = NULL;
	result = 1;
	if (argc != 2)
		return (0);
	if((fd1 = open(argv[1], O_RDONLY)) == -1)
		return (0);
	if((fd2 = open(argv[1], O_RDONLY)) == -1)
		return (0);
	if((fd3 = open(argv[1], O_RDONLY)) == -1)
		return (0);
	int i = 0;
	while (i < 6)
	{
		get_next_line(fd1, &line);
		printf("%s\n", line);
		get_next_line(fd1, &line);
		printf("%s\n", line);
		get_next_line(fd3, &line);
		printf("%s\n", line);
		i++;
	}
	get_next_line(fd1, NULL);
	if (line)
		free(line);
	close(fd1);
	return (0);
}
