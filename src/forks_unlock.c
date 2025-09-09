/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_unlock.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:57:33 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 13:58:04 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	forks_unlock(t_philo *philo, pthread_mutex_t **mtx_forks)
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
	return (0);
}
