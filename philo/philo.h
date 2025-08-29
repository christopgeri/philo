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

extern pthread_mutex_t 	hold_eating;
extern pthread_mutex_t 	hold_fork;
extern pthread_mutex_t 	hold_sleep;
extern pthread_mutex_t	hold;
extern pthread_mutex_t	hold_birth;
extern pthread_mutex_t	hold_wait;
extern pthread_mutex_t	hold_write;
extern pthread_mutex_t	hold_dead;
extern pthread_mutex_t	hold_time;
extern pthread_mutex_t	hold_lastmeal;
extern int				ms_output;
extern long int	micro_seconds;
extern long int	seconds;

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
	int	die;
	int	meals;
	int	meal_cnt;
	int n_philos;
	int	last_meal;
	int	*died;
	int	is_dead;
	int	current_time;
	int	left;
	int	right;
	int start;
	pthread_mutex_t *lock_fork;
}		t_philo;

typedef struct	s_philo_init
{
	int	id;
}		t_philo_init;

typedef struct s_thread_data
{
	t_args		*inpt_args;
	int			*wait_init;
}				t_thread_data;

typedef struct s_monitoring
{
	int				*forks;
	pthread_mutex_t	**mtx_forks;
	int				**last_meals;
	int				*died;
	int				*input;
}					t_monitoring;

typedef struct s_mtx
{
	pthread_mutex_t	init;
	pthread_mutex_t	sleep;
	pthread_mutex_t	eat;
	pthread_mutex_t	forks;
}					t_mtx;

int		ft_atoi(const char *nptr);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);

int		start_sleeping(t_philo *philo, pthread_mutex_t **mtx_forks);
int		start_eating(t_philo *philo, pthread_mutex_t **mtx_forks);
int		start_philo(t_philo *philo, pthread_mutex_t **mtx_forks);
void	*init_philo(void *arg);

int		lock_forks(t_philo *philo, pthread_mutex_t **mtx_forks);
int 	unlock_fork(t_philo *philo, pthread_mutex_t **mtx_forks);
int		time_for_timestamp(void);
int 	check_starved(int current_time, t_philo *philo);
pthread_t monitor_create(int **last_meals, pthread_mutex_t **mtx_forks, int *died, int *input);

int		write_status(int time, int id, char *str);

void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
size_t	ft_strlen(const char *s);

int buffer(int time, int id, int str);

#endif