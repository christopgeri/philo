/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 09:53:53 by cgeringe          #+#    #+#             */
/*   Updated: 2025/07/31 09:53:54 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_args
{
	int	n_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	n_of_meals;
}		t_args;

typedef struct	s_philo
{
	int	id;
	int	fork;
	int	eat;
	int sleep;
	int	think;
}		t_philo;

int	ft_atoi(const char *nptr);
void	ft_putnbr_fd(int n, int fd);

#endif