/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:37:23 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/06 15:37:25 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_STRUCTS_H
# define PHILO_STRUCTS_H

typedef struct s_args
{
	long int	n_of_philos;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	n_of_meals;
}		t_args;

typedef struct s_mtxes
{
	pthread_mutex_t	hold;
	pthread_mutex_t	hold_wait;
	pthread_mutex_t	hold_write;
	pthread_mutex_t	hold_dead;
	pthread_mutex_t	hold_time;
	pthread_mutex_t	hold_lastmeal;
	pthread_mutex_t	hold_buffer;
	int				initialized;
}					t_mtxes;

typedef struct s_thread_data
{
	t_args		*inpt_args;
	int			*wait_init;
	t_mtxes		*mtx;
}				t_thread_data;

typedef struct s_vals
{
	t_thread_data	*data;
	pthread_mutex_t	*mtx_forks[200];
	long int		*last_meals[200];
	long int		input[2];
	pthread_t		monitor;
	pthread_t		writer;
}					t_vals;

typedef struct s_philo
{
	int			id;
	long int	eat;
	long int	sleep;
	long int	die;
	int			meals;
	int			meal_cnt;
	int			n_philos;
	long int	last_meal;
	int			*died;
	int			is_dead;
	long int	current_time;
	int			left;
	int			right;
	long int	start;
	int			delay;
	int			sleep_loops;
	int			*created;
}				t_philo;

typedef struct s_philo_init
{
	int	id;
}		t_philo_init;

typedef struct s_monitoring
{
	int				*forks;
	pthread_mutex_t	**mtx_forks;
	long int		**last_meals;
	int				*died;
	long int		time_to_die;
	int				n_philos;
	long int		current_time;
	t_mtxes			*mtx;
}					t_monitoring;

typedef struct s_writer
{
	int				*status;
	t_mtxes			*mtx;
}					t_writer;

typedef struct s_flush_data
{
	long int	*times;
	int			*ids;
	int			*strs;
	int			i;
}					t_flush_data;

typedef struct s_buffer
{
	long int	times[2][1024];
	int			ids[2][1024];
	int			strs[2][1024];
	int			x;
	int			died;
	int			y;
}				t_buffer;

typedef struct s_cpy_buffer
{
	long int	times[1024];
	int			ids[1024];
	int			strs[1024];
	int			len;
	int			x;
	int			y;
}				t_cpy_buffer;

#endif