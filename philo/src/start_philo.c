/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:23 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:26 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// int lock_forks(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks);
int	check_starved(int current_time, t_philo *philo);
int check_times_eaten(t_philo *philo, int *meals);

/*
starts when all threads are created
and decides who eats first
*/
int	start_philo(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	if (philo->n_philos == 1)
	{
		philo->current_time = time_for_timestamp();
		pthread_mutex_lock(&hold_write);
		printf("%d %d has taken left fork\n", philo->current_time / ms_output, philo->id);
		usleep(philo->die);
		philo->current_time = time_for_timestamp();
		printf("%d %d died\n", philo->current_time / ms_output, philo->id);
		pthread_mutex_unlock(&hold_write);
		return (-1);
	}
	if (lock_forks(philo, mtx_forks) == 1)
		return (-1);
	start_eating(philo, mtx_forks);
	return (0);
}
