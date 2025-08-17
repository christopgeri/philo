#include "../philo.h"

int check_dead(int current_time, int last_meal, int time_to_die);

void *monitoring(void *thread_monitor)
{
	t_monitoring	*m_data; 
	int 			**last_meals;
	int				i;
	int				is_dead;
	int				current_time;
	int				time_to_die;
	int				n_philos;

	m_data = (t_monitoring *)thread_monitor;
	last_meals = m_data->last_meals;
	i = 0;
	time_to_die = m_data->input[0];
	n_philos = m_data->input[1];
	pthread_mutex_lock(&hold_time);
	while(seconds == 0 && micro_seconds == 0)
	{
		pthread_mutex_unlock(&hold_time);
		usleep(1000);
		pthread_mutex_lock(&hold_time);
	}
	pthread_mutex_unlock(&hold_time);
	while (i < n_philos)
	{
		current_time = time_for_timestamp();
		pthread_mutex_lock(&hold_lastmeal);
		is_dead = check_dead(current_time, *m_data->last_meals[i], time_to_die);
		pthread_mutex_unlock(&hold_lastmeal);
		if (is_dead == 1)
		{
			pthread_mutex_lock(&hold_write);
			pthread_mutex_lock(&hold_dead);
			*m_data->died = -1;
			printf("%d %d died! %d\n", current_time, i, *m_data->died);
			pthread_mutex_unlock(&hold_dead);
			pthread_mutex_unlock(&hold_write);
			return (NULL);
		}
		i++;
		if (i == n_philos)
			i = 0;
		pthread_mutex_lock(&hold_dead);
		if (*m_data->died == 1)
		{
			pthread_mutex_unlock(&hold_dead);
			return(NULL);
		}
		pthread_mutex_unlock(&hold_dead);
		usleep(1);
	}
	free(m_data);
	return (NULL);
}

int monitor_create(int **last_meals, int *forks, pthread_mutex_t **mtx_forks, int *died, int *input)
{
	pthread_t thread_monitor;
	t_monitoring *m_data;

	m_data = malloc(sizeof(t_monitoring));
	if (!m_data)
		return (1);
	m_data->forks = forks;
	m_data->last_meals = last_meals;
	m_data->mtx_forks = mtx_forks;
	pthread_mutex_lock(&hold_dead);
	m_data->died = died;
	pthread_mutex_unlock(&hold_dead);
	m_data->input = input;
	pthread_create(&thread_monitor, NULL, monitoring, (void*) m_data);
	pthread_detach(thread_monitor);
	return (0);
}

int check_dead(int current_time, int last_meal, int time_to_die)
{
	int time_since_last_meal;

	time_since_last_meal = current_time - last_meal;
	if (time_since_last_meal > time_to_die)
		return (1);
	return (0);
}