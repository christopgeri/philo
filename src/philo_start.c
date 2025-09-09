/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_start.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:23 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:26 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	one_philo(t_philo *philo, t_mtxes *mtx);

int	philo_start(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	if (philo->n_philos == 1)
		return (one_philo(philo, mtx));
	if (forks_lock(philo, mtx_forks, mtx) == 1)
		return (-1);
	philo_eat(philo, mtx_forks, mtx);
	return (0);
}

int	one_philo(t_philo *philo, t_mtxes *mtx)
{
	philo->current_time = timestamp(mtx);
	pthread_mutex_lock(&mtx->hold_write);
	buffer(philo->current_time, philo->id, 2, mtx);
	usleep(philo->die);
	philo->current_time = timestamp(mtx);
	buffer(philo->current_time, philo->id, 6, mtx);
	pthread_mutex_unlock(&mtx->hold_write);
	return (1);
}
