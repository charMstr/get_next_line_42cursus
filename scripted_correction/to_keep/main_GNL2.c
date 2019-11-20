/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_GNL2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 18:44:39 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/17 18:45:04 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
int	main(void)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*line;
	int		result;

	line = NULL;
	result = 1;
	if((fd1 = open("./file_linesA", O_RDONLY)) == -1)
		return (0);
	if((fd2 = open("./file_linesB", O_RDONLY)) == -1)
		return (0);
	if((fd3 = open("./file_linesC", O_RDONLY)) == -1)
		return (0);
	int i = 0;
	while (i < 6)
	{
		get_next_line(fd2, &line);
		printf("%s\n", line);
		get_next_line(fd2, &line);
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
