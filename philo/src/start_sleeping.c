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
int start_sleeping(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_sleep);
	// struct	timeval		tv;
	// struct	timezone	tz;
	int	current_time;

	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_sleep);
		return (-1);
	}
	if (philo->meal_cnt >= philo->meals)
	{
		philo->die = -2;
		pthread_mutex_unlock(&hold_sleep);
		return (1);
	}
	current_time = time_for_timestamp();
	// if (gettimeofday(&tv, &tz) == -1)
		// perror("gettimeofday: ");
	printf("%d %d %s\n", current_time, philo->id, "is sleeping");
	pthread_mutex_unlock(&hold_sleep);
	usleep(philo->sleep);
	if (*philo->died == 0)
	{
		// gettimeofday(&tv, &tz);
		current_time = time_for_timestamp();
		printf("%d %d %s\n", current_time, philo->id, "is thinking");
		start_philo(philo, philos_array, forks, mtx_forks);
	}
	return (0);
}
