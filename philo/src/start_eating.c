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
int start_eating(t_philo *philo, void **philos_array)
{
	pthread_mutex_lock(&hold_eating);
	struct	timeval		tv;
	struct	timezone	tz;

	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	philo->last_meal = (tv.tv_sec - seconds)*1000 + (tv.tv_usec - start_time)/ 1000 ;
	printf("%d %d %s\n", philo->last_meal, philo->id, "is eating");
	philo->meal_cnt++;
	pthread_mutex_unlock(&hold_eating);
	usleep(philo->eat);
	put_down_fork(philo, philos_array);
	start_sleeping(philo, philos_array);
	return (0);
}
