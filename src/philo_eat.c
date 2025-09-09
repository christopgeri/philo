/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:05 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:08 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	died_before_eating(t_philo *philo, pthread_mutex_t **mtx_forks,
		t_mtxes *mtx);
int	eat_check_dead(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx);

int	philo_eat(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	philo->current_time = timestamp(mtx);
	pthread_mutex_lock(&mtx->hold_lastmeal);
	if (philo->current_time - philo->last_meal >= philo->die)
		return (died_before_eating(philo, mtx_forks, mtx));
	if (philo->meals != -1 && philo->meal_cnt == philo->meals - 1)
		philo->last_meal = -1;
	else
		philo->last_meal = philo->current_time;
	pthread_mutex_unlock(&mtx->hold_lastmeal);
	pthread_mutex_lock(&mtx->hold_write);
	buffer(timestamp(mtx), philo->id, 3, mtx);
	pthread_mutex_unlock(&mtx->hold_write);
	if (eat_check_dead(philo, mtx_forks, mtx) == 1)
		return (1);
	philo->meal_cnt++;
	forks_unlock(philo, mtx_forks);
	philo_sleep(philo, mtx_forks, mtx);
	return (0);
}

int	died_before_eating(t_philo *philo, pthread_mutex_t **mtx_forks,
		t_mtxes *mtx)
{
	pthread_mutex_unlock(&mtx->hold_lastmeal);
	pthread_mutex_lock(&mtx->hold_dead);
	*philo->died = -1;
	pthread_mutex_unlock(&mtx->hold_dead);
	pthread_mutex_lock(&mtx->hold_write);
	buffer(philo->current_time, philo->id, 6, mtx);
	pthread_mutex_unlock(&mtx->hold_write);
	forks_unlock(philo, mtx_forks);
	return (1);
}

int	eat_check_dead(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	philo->sleep_loops = 0;
	if (philo->eat > 1000000)
		philo->sleep_loops = philo->eat / 1000000;
	if (philo->sleep_loops < 1)
		usleep(philo->eat);
	else
	{
		while (philo->sleep_loops > 0)
		{
			usleep(1000000);
			pthread_mutex_lock(&mtx->hold_dead);
			if (*philo->died == -1)
			{
				pthread_mutex_unlock(&mtx->hold_dead);
				forks_unlock(philo, mtx_forks);
				philo->is_dead = -1;
				return (1);
			}
			pthread_mutex_unlock(&mtx->hold_dead);
			philo->sleep_loops--;
		}
	}
	return (0);
}
