#include "../philo.h"

int check_dead(int current_time, int last_meal, int time_to_die);

void *monitoring(void *thread_monitor)
{
	t_monitoring	*m_data = (t_monitoring *)thread_monitor;
	int				n_philos = m_data->input[1];
	int				time_to_die = m_data->input[0] * 1000;
	int				i;
	int				current_time;
	int				is_dead;
	int				meal_time[200];

	pthread_mutex_lock(&hold_time);
	while (seconds == 0 && micro_seconds == 0) {
		pthread_mutex_unlock(&hold_time);
		usleep(500);
		pthread_mutex_lock(&hold_time);
	}
	pthread_mutex_unlock(&hold_time);
	usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&hold_lastmeal);
		current_time = time_for_timestamp();
		i = 0;
		while(i < n_philos)
		{
			meal_time[i] = *m_data->last_meals[i];
			i++;
		}
		pthread_mutex_unlock(&hold_lastmeal);
		i = 0;
		while (i < n_philos)
		{
			is_dead = check_dead(current_time, meal_time[i], time_to_die);
			if (is_dead >= 1)
			{
				pthread_mutex_lock(&hold_dead);
				*m_data->died = -1;
				pthread_mutex_lock(&hold_write);
				current_time = time_for_timestamp();
				buffer(current_time, i, 6);
				pthread_mutex_unlock(&hold_write);
				pthread_mutex_unlock(&hold_dead);
				free(m_data);
				return NULL;
			}
			i++;
		}
		pthread_mutex_lock(&hold_dead);
		if (*m_data->died == 1 || *m_data->died == -2)
		{
			*m_data->died = -1;
			pthread_mutex_unlock(&hold_dead);
			free(m_data);
			return NULL;
		}
		pthread_mutex_unlock(&hold_dead);
		usleep(100);
	}
	free(m_data);
	return NULL;
}

pthread_t monitor_create(int **last_meals, pthread_mutex_t **mtx_forks, int *died, int *input)
{
	pthread_t thread_monitor;
	t_monitoring *m_data;

	m_data = malloc(sizeof(t_monitoring));
	if (!m_data)
		return (1);
	pthread_mutex_lock(&hold_lastmeal);
	m_data->last_meals = last_meals;
	pthread_mutex_unlock(&hold_lastmeal);
	m_data->mtx_forks = mtx_forks;
	pthread_mutex_lock(&hold_dead);
	m_data->died = died;
	pthread_mutex_unlock(&hold_dead);
	m_data->input = input;
	pthread_create(&thread_monitor, NULL, monitoring, (void*) m_data);
	// pthread_detach(thread_monitor);
	return (thread_monitor);
}

int check_dead(int current_time, int last_meal, int time_to_die)
{
	int time_since_last_meal;

	if (last_meal == -1)
		return (0);
	time_since_last_meal = current_time - last_meal;
	if (time_since_last_meal > time_to_die)
	{
		// printf("CHECK_DEAD(): %d %d %d\n", time_since_last_meal, time_to_die, time_since_last_meal - time_to_die);
		return (1);
	}
	return (0);
}