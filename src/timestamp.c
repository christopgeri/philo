/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timestamp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:24:35 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 14:24:42 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long int	calc_time(struct timeval tv, long int start_sec,
				long int start_usec)
{
	long int	sec;
	long int	usec;
	long int	current_time;

	sec = tv.tv_sec - start_sec;
	usec = tv.tv_usec - start_usec;
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000;
	}
	current_time = (sec * 1000000) + usec;
	return (current_time);
}

long int	timestamp(t_mtxes *mtx)
{
	static long int		start_usec;
	static long int		start_sec;
	struct timeval		tv;
	struct timezone		tz;
	long int			current_time;

	pthread_mutex_lock(&mtx->hold_time);
	if (start_usec == 0 && start_sec == 0)
	{
		if (gettimeofday(&tv, &tz) == -1)
			perror("gettimeofday: ");
		start_usec = tv.tv_usec;
		start_sec = tv.tv_sec;
	}
	current_time = 0;
	gettimeofday(&tv, &tz);
	current_time = calc_time(tv, start_sec, start_usec);
	pthread_mutex_unlock(&mtx->hold_time);
	return (current_time);
}
