/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:36:54 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/03 15:36:56 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*writer(void *data)
{
	t_writer	*w_data;
	int			i;

	w_data = (t_writer *)data;
	i = 0;
	pthread_mutex_lock(&w_data->mtx->hold_dead);
	while (*w_data->status == 1)
	{
		pthread_mutex_unlock(&w_data->mtx->hold_dead);
		usleep(100);
		pthread_mutex_lock(&w_data->mtx->hold_dead);
	}
	pthread_mutex_unlock(&w_data->mtx->hold_dead);
	while (i == 0)
	{
		pthread_mutex_lock(&w_data->mtx->hold_write);
		if (buffer(0, -1, 0, w_data->mtx) == 1)
		{
			pthread_mutex_unlock(&w_data->mtx->hold_write);
			break ;
		}
		usleep(10);
	}
	return (NULL);
}

pthread_t	writer_create(t_mtxes *mtx, int *status)
{
	pthread_t		writer_thread;
	static t_writer	w_data;

	w_data.status = status;
	w_data.mtx = mtx;
	pthread_mutex_lock(&mtx->hold_dead);
	if (*status == -1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		return (0);
	}
	pthread_mutex_unlock(&mtx->hold_dead);
	if (pthread_create(&writer_thread, NULL, writer, (void *)&w_data) != 0)
	{
		ft_putstr_fd("Write thread creation failed!\n", 2);
		pthread_mutex_lock(&mtx->hold_dead);
		*status = -1;
		pthread_mutex_unlock(&mtx->hold_dead);
		return (0);
	}
	return (writer_thread);
}
