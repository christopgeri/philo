/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:17:31 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/03 12:17:37 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	buffer_reset(t_buffer *buf, int yindex)
{
	memset(buf->strs[yindex], -1, sizeof(buf->strs[yindex]));
	memset(buf->times[yindex], -1, sizeof(buf->times[yindex]));
	memset(buf->ids[yindex], -1, sizeof(buf->ids[yindex]));
	return (0);
}

static int	buffer_write_entry(long int time, int id, int str)
{
	ft_putnbr_fd(time / 1000, 1);
	ft_putchar_fd(' ', 1);
	ft_putnbr_fd(id, 1);
	if (str == 1)
		ft_putstr_fd(" has taken left fork\n", 1);
	else if (str == 2)
		ft_putstr_fd(" has taken right fork\n", 1);
	else if (str == 3)
		ft_putstr_fd(" is eating\n", 1);
	else if (str == 4)
		ft_putstr_fd(" is sleeping\n", 1);
	else if (str == 5)
		ft_putstr_fd(" is thinking\n", 1);
	else if (str == 6)
		ft_putstr_fd(" died\n", 1);
	return (0);
}

int	buffer_flush(long int *times, int *ids, int *strs, int i)
{
	int	j;
	int	died_time;

	j = 0;
	died_time = 0;
	if (i > 1024)
		i = 1024;
	if (i > 0 && strs[i - 1] == 6)
		died_time = times[i - 1];
	while (j < i)
	{
		if (times[j] == -1)
			break ;
		if (died_time && times[j] > died_time)
			j++;
		{
			buffer_write_entry(times[j], ids[j], strs[j]);
			j++;
		}
	}
	return (0);
}
