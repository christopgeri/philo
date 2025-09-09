/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:00:41 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 14:00:44 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int		monitor_wait(t_mtxes *mtx, int *died);
int		prepare_check_dead(t_monitoring *m_data, long int *meal_times);
int		creation_failed(t_vals *vals, t_mtxes *mtx);

void	*monitoring(void *thread_monitor)
{
	t_monitoring	*m_data;
	int				i;
	long int		meal_times[200];

	m_data = (t_monitoring *)thread_monitor;
	if (monitor_wait(m_data->mtx, m_data->died) == 1)
		return (NULL);
	usleep(m_data->time_to_die / 2);
	while (1)
	{
		prepare_check_dead(m_data, meal_times);
		i = 0;
		while (i < m_data->n_philos)
		{
			if (check_dead(m_data->current_time, meal_times[i],
					m_data->time_to_die) >= 1)
				return (monitor_dead_philo(m_data, i));
			i++;
		}
		usleep(500);
		if (monitor_done(m_data) == 1)
			return (NULL);
	}
	return (NULL);
}

pthread_t	monitor_create(t_vals *vals, t_mtxes *mtx)
{
	pthread_t			thread_monitor;
	static t_monitoring	m_data;

	pthread_mutex_lock(&mtx->hold_lastmeal);
	m_data.last_meals = vals->last_meals;
	pthread_mutex_unlock(&mtx->hold_lastmeal);
	m_data.mtx_forks = vals->mtx_forks;
	pthread_mutex_lock(&mtx->hold_dead);
	if (*vals->data->wait_init == -1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		return (0);
	}
	m_data.died = vals->data->wait_init;
	pthread_mutex_unlock(&mtx->hold_dead);
	m_data.n_philos = vals->input[1];
	m_data.time_to_die = vals->input[0] * 1000;
	m_data.current_time = 0;
	m_data.mtx = mtx;
	if (pthread_create(&thread_monitor, NULL, monitoring, (void *)&m_data) != 0)
		return (creation_failed(vals, mtx));
	return (thread_monitor);
}

int	monitor_wait(t_mtxes *mtx, int *died)
{
	pthread_mutex_lock(&mtx->hold_dead);
	while (*died == 1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		usleep(100);
		pthread_mutex_lock(&mtx->hold_dead);
	}
	if (*died == -1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		return (1);
	}
	pthread_mutex_unlock(&mtx->hold_dead);
	return (0);
}

int	prepare_check_dead(t_monitoring *m_data, long int *meal_times)
{
	int	i;

	i = 0;
	m_data->current_time = timestamp(m_data->mtx);
	pthread_mutex_lock(&m_data->mtx->hold_lastmeal);
	while (i < m_data->n_philos)
	{
		meal_times[i] = *m_data->last_meals[i];
		i++;
	}
	pthread_mutex_unlock(&m_data->mtx->hold_lastmeal);
	return (0);
}

int	creation_failed(t_vals *vals, t_mtxes *mtx)
{
	ft_putstr_fd("Monitor thread creation failed!\n", 2);
	pthread_mutex_lock(&mtx->hold_dead);
	*vals->data->wait_init = -1;
	pthread_mutex_unlock(&mtx->hold_dead);
	return (0);
}
