#include "../philo.h"

int lock_forks_first(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int id);
int lock_forks_not_first(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int id);

/* lock the fork to the left first(own), then the one to the right (philo id - 1)*/
int lock_forks(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_unlock(&hold_fork);
	int	id;

	id = philo->id;
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	if (philo->id == 0)
		return (lock_forks_first(philo, forks, mtx_forks, id));
	else
		return (lock_forks_not_first(philo, forks, mtx_forks, id));
	return (0);
}

int lock_forks_first(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int id)
{
	pthread_mutex_lock(mtx_forks[id]);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(mtx_forks[id]);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
		printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken left fork", id);
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	forks[id] = 0;
	philo->fork++;
	pthread_mutex_lock(mtx_forks[philo->n_philos - 1]);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(mtx_forks[id]);
		pthread_mutex_unlock(mtx_forks[philo->n_philos - 1]);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
		printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken right fork", philo->n_philos - 1);
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	forks[philo->n_philos - 1] = 0;
	philo->fork++;
	pthread_mutex_lock(&hold_fork);
	return(philo->fork);
}

int lock_forks_not_first(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int id)
{
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(mtx_forks[id]);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(mtx_forks[id]);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
		printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken left fork", id);
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	forks[id] = 0;
	philo->fork++;
	pthread_mutex_lock(mtx_forks[id - 1]);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		pthread_mutex_unlock(mtx_forks[id]);
		pthread_mutex_unlock(mtx_forks[id - 1]);
		return (1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
	printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken right fork", id - 1);
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	forks[id - 1] = 0;
	philo->fork++;
	pthread_mutex_lock(&hold_fork);
	return(philo->fork);
}

// int lock_forks_not_first(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int id)
// {
// 	pthread_mutex_lock(&hold_dead);
// 	if (*philo->died == -1)
// 	{
// 		pthread_mutex_unlock(&hold_dead);
// 		return (1);
// 	}
// 	pthread_mutex_unlock(&hold_dead);
// 	// if (philo->id % 2 == 1)
// 	// {
// 		pthread_mutex_lock(mtx_forks[id]);
// 		pthread_mutex_lock(&hold_dead);
// 		if (*philo->died == -1)
// 		{
// 			pthread_mutex_unlock(&hold_dead);
// 			pthread_mutex_unlock(mtx_forks[id]);
// 			return (1);
// 		}
// 		pthread_mutex_unlock(&hold_dead);
// 		pthread_mutex_lock(&hold_write);
// 		printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken left fork", id);
// 		pthread_mutex_unlock(&hold_write);
// 		forks[id] = 0;
// 		philo->fork++;
// 		pthread_mutex_lock(mtx_forks[id - 1]);
// 		pthread_mutex_lock(&hold_dead);
// 		if (*philo->died == -1)
// 		{
// 			pthread_mutex_unlock(&hold_dead);
// 			pthread_mutex_unlock(mtx_forks[id - 1]);
// 			pthread_mutex_unlock(mtx_forks[id]);
// 			return (1);
// 		}
// 		pthread_mutex_unlock(&hold_dead);
// 		pthread_mutex_lock(&hold_write);
// 		printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken right fork", id - 1);
// 		pthread_mutex_unlock(&hold_write);
// 		forks[id - 1] = 0;
// 		philo->fork++;
// 	// }
// 	// else if (philo->id % 2 == 0)
// 	// {
// 	// 	pthread_mutex_lock(mtx_forks[id - 1]);
// 	// 	pthread_mutex_lock(&hold_dead);
// 	// 	if (*philo->died == -1)
// 	// 	{
// 	// 		pthread_mutex_unlock(&hold_dead);
// 	// 		pthread_mutex_unlock(mtx_forks[id - 1]);
// 	// 		return (1);
// 	// 	}
// 	// 	pthread_mutex_unlock(&hold_dead);
// 	// 	pthread_mutex_lock(&hold_write);
// 	// 	printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken right fork", id - 1);
// 	// 	pthread_mutex_unlock(&hold_write);
// 	// 	forks[id - 1] = 0;
// 	// 	philo->fork++;
// 	// 	pthread_mutex_lock(mtx_forks[id]);
// 	// 	pthread_mutex_lock(&hold_dead);
// 	// 	if (*philo->died == -1)
// 	// 	{
// 	// 		pthread_mutex_unlock(&hold_dead);
// 	// 		pthread_mutex_unlock(mtx_forks[id]);
// 	// 		pthread_mutex_unlock(mtx_forks[id - 1]);
// 	// 		return (1);
// 	// 	}
// 	// 	pthread_mutex_unlock(&hold_dead);
// 	// 	pthread_mutex_lock(&hold_write);
// 	// 	printf("%d %d %s (%d)\n", time_for_timestamp(), philo->id, "has taken left fork", id);
// 	// 	pthread_mutex_unlock(&hold_write);
// 	// 	forks[id] = 0;
// 	// 	philo->fork++;
// 	// }
// 	pthread_mutex_lock(&hold_fork);
// 	return(philo->fork);
// }