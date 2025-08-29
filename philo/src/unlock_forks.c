#include "../philo.h"

int unlock_fork(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(mtx_forks[philo->right]);
		pthread_mutex_unlock(mtx_forks[philo->left]);
	}
	else
	{
		pthread_mutex_unlock(mtx_forks[philo->left]);
		pthread_mutex_unlock(mtx_forks[philo->right]);
	}
	return(0);
}
