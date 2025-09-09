/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:37:12 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/03 15:37:27 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	monitor_done(t_monitoring *m_data)
{
	pthread_mutex_lock(&m_data->mtx->hold_dead);
	if (*m_data->died == 1 || *m_data->died == -1)
	{
		*m_data->died = -1;
		pthread_mutex_unlock(&m_data->mtx->hold_dead);
		return (1);
	}
	pthread_mutex_unlock(&m_data->mtx->hold_dead);
	return (0);
}

void	*monitor_dead_philo(t_monitoring *m_data, int i)
{
	pthread_mutex_lock(&m_data->mtx->hold_dead);
	*m_data->died = -1;
	pthread_mutex_unlock(&m_data->mtx->hold_dead);
	pthread_mutex_lock(&m_data->mtx->hold_write);
	buffer(m_data->current_time, i, 6, m_data->mtx);
	pthread_mutex_unlock(&m_data->mtx->hold_write);
	return (NULL);
}

int	check_dead(long int current_time, long int last_meal, long int time_to_die)
{
	long int	time_since_last_meal;

	if (last_meal == -1)
		return (0);
	else
	{
		time_since_last_meal = current_time - last_meal;
		if (time_since_last_meal >= time_to_die)
			return (1);
	}
	return (0);
}
