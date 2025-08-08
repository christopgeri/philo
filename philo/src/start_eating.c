/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:05 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:08 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
philo starts eating for the given time
and then calls the sleeping function
*/
int start_eating(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_eating);
	// struct	timeval		tv;
	// struct	timezone	tz;
	// int					current_time;
	if (*philo->died == -1)
	{
		unlock_fork(philo, philos_array, forks, mtx_forks);
		pthread_mutex_unlock(&hold_sleep);
		return (-1);
	}
	// if (gettimeofday(&tv, &tz) == -1)
		// perror("gettimeofday: ");
	// current_time = time_for_timestamp();
	philo->last_meal = time_for_timestamp();
	// philo->last_meal = (tv.tv_sec - seconds) * 1000 + (tv.tv_usec - micro_seconds) / 1000 ;
	printf("%d %d %s\n", philo->last_meal, philo->id, "is eating");
	philo->meal_cnt++;
	pthread_mutex_unlock(&hold_eating);
	usleep(philo->eat);
	unlock_fork(philo, philos_array, forks, mtx_forks);
	start_sleeping(philo, philos_array, forks, mtx_forks);
	return (0);
}
