#include "../philo.h"

int lock_left(t_philo *philo, pthread_mutex_t **mtx_forks);
int lock_right(t_philo *philo, pthread_mutex_t **mtx_forks);

int lock_forks(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	if (philo->id % 2 == 0)
	{
		if (lock_right(philo, mtx_forks) == 1)
			return (1);
		if (lock_left(philo, mtx_forks) == 1)
			return (1);
	}
	else
	{
		if (lock_left(philo, mtx_forks) == 1)
			return (1);
		if (lock_right(philo, mtx_forks) == 1)
			return (1);
	}
	return (2);
}

int lock_left(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(mtx_forks[philo->left]);
	pthread_mutex_lock(&hold_write);
	philo->current_time = time_for_timestamp();
	if (philo->is_dead == 0)
	{
		buffer(philo->current_time, philo->id, 1);
		pthread_mutex_unlock(&hold_write);
	}
	else
	{
		pthread_mutex_unlock(&hold_write);
		pthread_mutex_unlock(mtx_forks[philo->left]);
		if (philo->id % 2 == 0)
			pthread_mutex_unlock(mtx_forks[philo->right]);
		return (1);
	}
	philo->fork++;
	return (0);
}

int lock_right(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(mtx_forks[philo->right]);
	pthread_mutex_lock(&hold_write);
	philo->current_time = time_for_timestamp();
	if (philo->is_dead == 0)
	{
		buffer(philo->current_time, philo->id, 2);
		pthread_mutex_unlock(&hold_write);
	}
	else
	{
		pthread_mutex_unlock(&hold_write);
		pthread_mutex_unlock(mtx_forks[philo->right]);
		if (philo->id %2 != 0)
			pthread_mutex_unlock(mtx_forks[philo->left]);
		return (1);
	}
	philo->fork++;
	return (0);
}
