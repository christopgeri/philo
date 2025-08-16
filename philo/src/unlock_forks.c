#include "../philo.h"

int unlock_fork(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks)
{
	int	id;

	id = philo->id;
	if (id == 0)
	{
		forks[philo->n_philos - 1] = 1;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[philo->n_philos - 1]);
		forks[id] = 1;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id]);
		return(philo->fork);
	}
	else
	{
		forks[id - 1] = 1;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id-1]);
		forks[id] = 1;
		philo->fork--;
		pthread_mutex_unlock(mtx_forks[id]);
		return(philo->fork);
	}
	return (0);
}
