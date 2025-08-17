/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sleeping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:37 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:40 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
philo starts sleepin for given time
*/
int start_sleeping(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_sleep);
	int	current_time;

	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(&hold_sleep);
		return (-1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_write);
	current_time = time_for_timestamp();
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
		printf("%d %d %s\n", current_time, philo->id, "is sleeping");
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	pthread_mutex_unlock(&hold_sleep);
	usleep(philo->sleep);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == 0)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_lock(&hold_sleep);
		current_time = time_for_timestamp();
		pthread_mutex_lock(&hold_write);
		pthread_mutex_lock(&hold_dead);
		if (*philo->died != -1)
			printf("%d %d %s\n", current_time, philo->id, "is thinking");
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(&hold_write);
		pthread_mutex_unlock(&hold_sleep);
		if (philo->meal_cnt >= philo->meals)
			return (1);
		start_philo(philo, forks, mtx_forks);
	}
	else
		pthread_mutex_unlock(&hold_dead);
	return (0);
}
