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
int start_sleeping(t_philo *philo, void **philos_array)
{
	pthread_mutex_lock(&hold_sleep);
	struct	timeval		tv;
	struct	timezone	tz;

	if (philo->meal_cnt >= philo->meals)
	{
		printf("  PHILO %d had enough meals!\n", philo->id);
		pthread_mutex_unlock(&hold_sleep);
		return (1);
	}
	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	printf("%ld %d %s\n", (tv.tv_sec - seconds)*1000 + (tv.tv_usec - start_time) / 1000, philo->id, "is sleeping");
	pthread_mutex_unlock(&hold_sleep);
	usleep(philo->sleep);
	if (philo->meal_cnt < philo->meals)
		start_philo(philo, philos_array);
	return (0);
}
