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

int take_fork(t_philo *philo, void **philos_array);

/*
starts when all threads are created
and decides who eats first
*/
int	start_philo(t_philo *philo, void **philos_array)
{
	if (pthread_mutex_lock(&hold_fork) != 0)
		perror("mutex lock failed: ");

	struct	timeval		tv;
	struct	timezone	tz;
	int					current_time;
	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	if (start_time == 0)
		start_time = tv.tv_usec;
	if (seconds == 0)
		seconds = tv.tv_sec;
	current_time = (tv.tv_sec - seconds) * 1000 + (tv.tv_usec - start_time) / 1000;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->lock_fork);
		take_fork(philo, philos_array);
		if(philo->fork == 2)
			printf("%d %d %s\n",current_time, philo->id, "has taken a fork");
		else
			printf("%d %d %s\n",
				current_time, philo->id, "is_thinking");
		pthread_mutex_unlock(philo->lock_fork);
	}
	else
	{
		if (philo->fork == 2)
			printf("%d %d %s\n", current_time, philo->id, "has taken a fork");
		else
			printf("%d %d %s\n",
				current_time, philo->id, "is_thinking");
	}
	pthread_mutex_unlock(&hold_fork);
	if (philo->fork == 2)
		start_eating(philo, philos_array);
	else
	{
		usleep(philo->eat);
		while(philo->fork == 0)
			usleep(10);
		start_philo(philo, philos_array);
	}
	return (0);
}

// int take_fork(t_philo *philo, t_thread_data *data)
int take_fork(t_philo *philo, void **philos_array)
{
	t_philo	*philo_left;
	// printf("TAKE_FORK()\n");
	if (philos_array)
	{
		philo_left = (t_philo *) philos_array[philo->id - 2];
		// printf("\tAddr[philo_left]. %p\n", philo_left);
		if (philo_left) //  && philo_left->id == philo->id -1
		{
			// printf("\tphilo_left->id = %d\n\tphilo_right->id = %d\n",
				// philo_left->id, philo->id);
			if (philo_left->fork == 1)
			{
				philo_left->fork = 0;
				philo->fork = 2;
				return (2);
			}
		}
	}
	return (0);
}

int put_down_fork(t_philo *philo, void **philos_array)
{
	t_philo	*philo_left;
	// printf("DOWN_FORK()\n");
	if (philos_array)
	{
		if (philo->id == 1)
			philo_left = (t_philo *) philos_array[philo->n_philos - 1];
		else
			philo_left = (t_philo *) philos_array[philo->id - 2];
		// printf("\tAddr[philo_left]. %p\n", philo_left);
		if (philo_left) //  && philo_left->id == philo->id -1
		{
			// printf("\tphilo_left->id = %d\n\tphilo_right->id = %d\n",
				// philo_left->id, philo->id);
			if (philo_left->fork == 0)
			{
				philo_left->fork = 2;
				philo->fork = 0;
				return (0);
			}
		}
		philo->fork = 1;
	}
	return (2);
}

	// printf("%d + %d >= %d\n", philo->die / 1000, philo->last_meal, current_time);
	// if (philo->die / 1000 + philo->last_meal >= current_time && philo->last_meal > 0) // find right calculation
	// {
	// 	printf("%d ms Philo %d %s\n",
	// 			current_time, philo->id, "died!");
	// 	pthread_mutex_unlock(&hold_fork);
	// 	return (1);
	// }

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