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
int	start_philo(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks)
{
	if (pthread_mutex_lock(&hold_fork) != 0)
		perror("mutex lock failed: ");

	struct	timeval		tv;
	struct	timezone	tz;
	int					current_time;
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(&hold_fork);
		return (-1);
	}
	// printf("%d %d", philo->id, *philo->died);
	pthread_mutex_unlock(&hold_dead);
	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	pthread_mutex_lock(&hold_time);
	if (micro_seconds == 0 && seconds == 0)
	{
		micro_seconds = tv.tv_usec;
		seconds = tv.tv_sec;
	}
	pthread_mutex_unlock(&hold_time);
	if (philo->n_philos == 1)
	{
		current_time = time_for_timestamp();
		usleep(philo->die);
		pthread_mutex_lock(&hold_write);
		printf("%d %d died\n", current_time, philo->id);
		pthread_mutex_unlock(&hold_write);
		return (-1);
	}
	if (lock_forks(philo, forks, mtx_forks) == 1)
		return (-1);
	current_time = time_for_timestamp();
	pthread_mutex_unlock(&hold_fork);
	if (philo->fork == 2)
		start_eating(philo, forks, mtx_forks);
	return (0);
}

int check_starved(int current_time, t_philo *philo)
{
	int	last_meal;
	int	time_to_die;
	int time_since_last_meal;

	last_meal = philo->last_meal;
	time_to_die = philo->die / 1000;
	time_since_last_meal = current_time - last_meal;
	if (time_since_last_meal > time_to_die)
	{
		*philo->died = -1;
		return (1);
	}
	return (0);
}

int check_times_eaten(t_philo *philo, int *meals)
{
	int i;

	i = 0;
	while (i < philo->n_philos)
	{
		if (meals[i] < philo->meal_cnt)
			return (1);
		i++;
	}
	return (0);
}

	// printf("START_PHILO()\n");
// TEST
	// t_philo *testphilo;
	// int y = 0;
	// while (y < 4){
	// 	testphilo = (t_philo *) philos_array[y++];
	// 	printf("Addr[%d]. %p, ", y, testphilo);
	// }
	// printf("\n\n");
// TEST END
