/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:24:07 by cgeringe          #+#    #+#             */
/*   Updated: 2025/07/29 18:33:27 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

pthread_t	birth_of_philo(t_thread_data *data)
{
	pthread_t	thread_n;

	pthread_mutex_lock(&data->mtx->hold_dead);
	if (*data->wait_init == -1)
	{
		pthread_mutex_unlock(&data->mtx->hold_dead);
		return (0);
	}
	pthread_mutex_unlock(&data->mtx->hold_dead);
	if (pthread_create(&thread_n, NULL, &init_philo, (void *)data) != 0)
	{
		ft_putstr_fd("philo thread creation failed!\n", 2);
		pthread_mutex_lock(&data->mtx->hold_dead);
		*data->wait_init = -1;
		pthread_mutex_unlock(&data->mtx->hold_dead);
		return (0);
	}
	usleep(1000);
	return (thread_n);
}

int	init_mutexes(t_mtxes *mtx)
{
	mtx->initialized = 0;
	if (pthread_mutex_init(&mtx->hold, NULL) != 0)
		return (1);
	mtx->initialized = 1;
	if (pthread_mutex_init(&mtx->hold_wait, NULL) != 0)
		return (2);
	mtx->initialized = 2;
	if (pthread_mutex_init(&mtx->hold_write, NULL) != 0)
		return (3);
	mtx->initialized = 3;
	if (pthread_mutex_init(&mtx->hold_dead, NULL) != 0)
		return (4);
	mtx->initialized = 4;
	if (pthread_mutex_init(&mtx->hold_time, NULL) != 0)
		return (5);
	mtx->initialized = 5;
	if (pthread_mutex_init(&mtx->hold_lastmeal, NULL) != 0)
		return (6);
	mtx->initialized = 6;
	if (pthread_mutex_init(&mtx->hold_buffer, NULL) != 0)
		return (7);
	mtx->initialized = 7;
	return (0);
}

void	join_philo_threads(pthread_t *thread_ids, int created)
{
	int	returned;

	returned = 0;
	while (returned < created)
	{
		if (thread_ids[returned] != 0)
			pthread_join(thread_ids[returned], NULL);
		returned++;
	}
}

int	create_philos(t_args *args)
{
	int				created;
	int				control_status;
	pthread_t		thread_ids[200];
	t_thread_data	data;
	t_mtxes			mtx;

	created = 0;
	control_status = 1;
	data.inpt_args = args;
	data.wait_init = &control_status;
	data.mtx = &mtx;
	mtx.initialized = init_mutexes(&mtx);
	if (mtx.initialized > 0)
		return (destroy_mutexes(&mtx));
	while (created < args->n_of_philos)
		thread_ids[created++] = birth_of_philo(&data);
	usleep(10000);
	pthread_mutex_lock(&mtx.hold_dead);
	if (control_status != -1)
		control_status = 0;
	pthread_mutex_unlock(&mtx.hold_dead);
	join_philo_threads(thread_ids, created);
	return (destroy_mutexes(&mtx));
}

int	main(int argc, char *argv[])
{
	static t_args	args;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("5 or 6 arguments required!\n", 2);
		return (1);
	}
	if (!parsing_init(&args, argc, argv))
		return (1);
	buffer(0, -2, 0, NULL);
	create_philos(&args);
	return (0);
}
