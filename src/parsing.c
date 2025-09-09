/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:59:42 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 13:59:45 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"
#include <limits.h>

int	overlfow_check(long int num, char *arg, int i, int *error);

long int	conv_arg_to_long(char *arg, int *error)
{
	long int	num;
	int			i;

	num = 0;
	i = 0;
	if (arg[0] == '\0')
	{
		*error = 1;
		return (0);
	}
	if (arg[i] == '+')
		i++;
	while (arg[i] >= '0' && arg[i] <= '9')
	{
		if (overlfow_check(num, arg, i, error) == 1)
			return (0);
		num = num * 10 + (arg[i] - 48);
		i++;
	}
	if (arg[i] != '\0')
		*error = 1;
	return (num);
}

int	overlfow_check(long int num, char *arg, int i, int *error)
{
	if (num > LONG_MAX / 10)
	{
		*error = 2;
		return (1);
	}
	else if (num == LONG_MAX && (arg[i]) > LONG_MAX % 10)
	{
		*error = 2;
		return (1);
	}
	return (0);
}

int	invalid_input(int error, t_args *args)
{
	int	invalid;

	invalid = 0;
	if (error == 1)
	{
		ft_putstr_fd("Error: Arguments must be positive integers!\n", 2);
		invalid = 1;
	}
	if (error == 2)
	{
		ft_putstr_fd("Error: Argument overflow!\n", 2);
		invalid = 1;
	}
	if (args->n_of_philos < 1 || args->n_of_philos > 200)
	{
		ft_putstr_fd("Error: Chose between 1 and 200 philos\n", 2);
		invalid = 1;
	}
	if (args->n_of_meals == 0)
	{
		ft_putstr_fd("Error: Number of meals must be at least 1\n", 2);
		invalid = 1;
	}
	return (invalid);
}

int	parsing_init(t_args *args, int argc, char *argv[])
{
	int		error;

	error = 0;
	args->n_of_philos = conv_arg_to_long(argv[1], &error);
	args->time_to_die = conv_arg_to_long(argv[2], &error);
	args->time_to_eat = conv_arg_to_long(argv[3], &error);
	args->time_to_sleep = conv_arg_to_long(argv[4], &error);
	if (argc == 6)
		args->n_of_meals = conv_arg_to_long(argv[5], &error);
	else
		args->n_of_meals = -1;
	if (invalid_input(error, args))
		return (0);
	return (1);
}
