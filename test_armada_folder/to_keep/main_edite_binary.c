/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_edite_binary.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:37:22 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/17 16:39:51 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
int	main(void)
{
	int		fd1;
	char	*line;
	int	bytes_read;
	int		result;

	line = NULL;
	result = 1;
	if((fd1 = open("./a.out_test", O_RDONLY)) == -1)
		return (0);
	int i = 0;
	while (get_next_line(fd1, &line) > 0)
		printf("%s\n", line);
	printf("%s\n", line);
	get_next_line(fd1, NULL);
	if (line)
		free(line);
	close(fd1);
	return (0);
}
