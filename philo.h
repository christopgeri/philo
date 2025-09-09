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
# define PHILO_H
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include "philo_structs.h"

long int	conv_arg_to_long(char *arg, int *error);
int			invalid_input(int error, t_args *args);
int			parsing_init(t_args *args, int argc, char *argv[]);

void		*init_philo(void *arg);
int			init_philo_return(t_philo *philo, t_vals *vals, t_mtxes *mtx);
int			init_check_dead(t_philo *philo, t_mtxes *mtx);
void		create_help_threads(t_vals *vals, t_mtxes *mtx);

int			philo_sleep(t_philo *philo, pthread_mutex_t **mtx_forks,
				t_mtxes *mtx);
int			philo_eat(t_philo *philo, pthread_mutex_t **mtx_forks,
				t_mtxes *mtx);
int			philo_start(t_philo *philo, pthread_mutex_t **mtx_forks,
				t_mtxes *mtx);
int			philo_think(t_philo *philo, pthread_mutex_t **mtx_forks,
				t_mtxes *mtx);

int			forks_lock(t_philo *philo, pthread_mutex_t **mtx_forks,
				t_mtxes *mtx);
int			forks_unlock(t_philo *philo, pthread_mutex_t **mtx_forks);

long int	timestamp(t_mtxes *mtx);

pthread_t	monitor_create(t_vals *vals, t_mtxes *mtx);
int			check_dead(long int current_time, long int last_meal,
				long int time_to_die);
void		*monitor_dead_philo(t_monitoring *m_data, int i);
int			monitor_done(t_monitoring *m_data);

pthread_t	writer_create(t_mtxes *mtx, int *status);

int			buffer(long int time, int id, int str, t_mtxes *mtx);
int			buffer_flush(long int *time, int *ids, int *strs, int i);
int			buffer_reset(t_buffer *buf, int yindex);

int			destroy_mutexes(t_mtxes *mtx);

void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
size_t		ft_strlen(const char *s);

#endif