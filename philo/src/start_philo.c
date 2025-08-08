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

int lock_forks(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks);
int	check_starved(int current_time, t_philo *philo);

/*
starts when all threads are created
and decides who eats first
*/
int	start_philo(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
{
	if (pthread_mutex_lock(&hold_fork) != 0)
		perror("mutex lock failed: ");

	struct	timeval		tv;
	struct	timezone	tz;
	int					current_time;
	if (*philo->died == -1)
	{
		// printf("%d start_philo\n", philo->id);
		pthread_mutex_unlock(&hold_sleep);
		return (-1);
	}
	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	if (micro_seconds == 0)
		micro_seconds = tv.tv_usec;
	if (seconds == 0)
		seconds = tv.tv_sec;
	current_time = time_for_timestamp();
	// current_time = (tv.tv_sec - seconds) * 1000 + (tv.tv_usec - micro_seconds) / 1000;
	if (check_starved(current_time, philo) == 1)
	{
		printf("%d %d %s\n", current_time, philo->id, "died");
		pthread_mutex_unlock(&hold_fork);
		return (-1);
	}
	lock_forks(philo, philos_array, forks, mtx_forks);
	current_time = time_for_timestamp();
	if (philo->fork == 2)
		printf("%d %d %s\n", current_time, philo->id, "has taken a fork");
	else
		printf("%d %d %s\n", current_time, philo->id, "is_thinking");
	pthread_mutex_unlock(&hold_fork);
	if (philo->fork == 2)
		start_eating(philo, philos_array, forks, mtx_forks);
	else
	{
		printf("here\n");
		usleep(philo->eat);
		start_philo(philo, philos_array, forks, mtx_forks);
	}
	return (0);
}

// int take_fork(t_philo *philo, t_thread_data *data)
int lock_forks(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
{
	int	id;

	id = philo->id;
	if (!philos_array)
		return (-1);
	if (id == 0)
	{
		// printf("Lock fork %d\n", philo->id);
		pthread_mutex_lock(mtx_forks[philo->n_philos - 1]);
		forks[philo->n_philos - 1] = 0;
		philo->fork++;
		pthread_mutex_lock(mtx_forks[id]);
		forks[id] = 0;
		philo->fork++;
		return(philo->fork);
	}
	else
	{
		// printf("Lock fork %d\n", philo->id);
		pthread_mutex_lock(mtx_forks[id]);
		forks[id] = 0;
		philo->fork++;
		pthread_mutex_lock(mtx_forks[id - 1]);
		forks[id - 1] = 0;
		philo->fork++;
		return(philo->fork);
	}
	return (0);
}

int unlock_fork(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
{
	int	id;

	id = philo->id;
	if (!philos_array)
		return (-1);
	if (id == 0)
	{
		// printf("Unlock fork %d\n", philo->id);
		forks[philo->n_philos - 1] = 0;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[philo->n_philos - 1]);
		forks[id] = 0;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id]);
		return(philo->fork);
	}
	else
	{
		// printf("Unlock fork %d\n", philo->id);
		forks[id] = 0;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id]);
		forks[id - 1] = 0;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id - 1]);
		return(philo->fork);
	}
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
		printf("%d - %d = %d > %d\n", current_time, last_meal, time_since_last_meal, time_to_die);
		return (1);
	}
	// printf("%d > %d\n", time_since_last_meal, time_to_die);
	return (0);
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

// int lock_forks(t_philo *philo, void **philos_array, int *forks, pthread_mutex_t **mtx_forks)
// {
// 	t_philo	*philo_right;
// 	// printf("TAKE_FORK()\n");
// 	if (philos_array)
// 	{
// 		if (philo->id == philo->n_philos - 1)
// 			philo_right = (t_philo *) philos_array[0];
// 		else
// 			philo_right = (t_philo *) philos_array[philo->id + 1];
// 		// printf("\tAddr[philo_right]. %p\n", philo_right);
// 		if (philo_right) //  && philo_right->id == philo->id -1
// 		{
// 			// printf("\tphilo_left->id = %d\n\tphilo_right->id = %d\n",
// 				// philo_right->id, philo->id);
// 			if (philo_right->fork == 1)
// 			{
// 				philo_right->fork = 0;
// 				philo->fork = 2;
// 				return (2);
// 			}
// 		}
// 	}
// 	return (0);
// }