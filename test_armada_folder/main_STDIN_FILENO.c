/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_STDIN_FILENO.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 22:08:56 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/19 13:32:56 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_STDIN_FILENO.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 19:50:33 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/18 22:06:12 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/get_next_line_tests_utils.h"

int	main(int argc, char *argv[])
{
	char	*line;
	int		result;
	char 	buf[BUFFER_SIZE + 1];

	line = NULL;
	result = 1;
	argv[0] = (char *)argv[0];
	if (argc != 2)
	{
		ft_putstr_fd("failed to provide the file to be opened as arg\n", 2);
		return (0);
	}
	ft_putstr_fd("\n\n\t\t\tlets see how your GNL behaves with the STDIN_FILENO:\n", 1);

	while (result > 0)
	{
		result = get_next_line(STDIN_FILENO, &line);
		if (result != -1)
			ft_putstr_fd(line, 1);
	}
	ft_putstr_fd("\nyour result was:", 1);
	ft_putnbr_fd(result, 1);
	ft_putstr_fd("\n\n\t\t\tnow compare with read behavior:\n", 1);
	result = 1;
	while (result > 0)
	{
		result = read(STDIN_FILENO, buf, BUFFER_SIZE);
		if (result != -1)
		{
			buf[result] = '\0';
			ft_putstr_fd(buf, 1);
		}
		result = 0;
	}
	free(line);
	return (0);
}

