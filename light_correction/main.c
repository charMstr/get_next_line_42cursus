/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 16:47:28 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/20 16:31:50 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "get_next_line_tests_utils.h"

int	main(int argc, char *argv[])
{
	int 	fd_test_me;
	char	*line;
	int		result;

	line = NULL;
	result = 1;
	argv[0] = (char *)argv[0];
	if (argc != 2)
	{
		ft_putstr_fd("failed to provide the file to be opened as arg\n", 2);
		return (0);
	}
	if((fd_test_me = open(argv[1], O_RDONLY)) == -1)
	{
		ft_putstr_fd("failed to open", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("filedescriptor.\n", 2);
		return (0);
	}
	while (result > 0)
	{
		result = get_next_line(fd_test_me, &line);
		if (result != -1)
		{
			ft_putstr_fd(line, 1);
			ft_putchar_fd('\n', 1);
		}
	}
	close(fd_test_me);
	free(line);
	return (0);
}
