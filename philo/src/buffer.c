#include "../philo.h"

int	flush_buffer(int *time, int *ids, int *strs, int i);

int buffer(int time, int id, int str)
{
	static int	strs[4096 * 5];
	static int	times[4096 * 5];
	static int	ids[4096 * 5];
	static int	i;
	static int	died;

	if (time == -2)
	{
		i = 0;
		died = 0;
		memset(strs, -1, sizeof(strs));
		memset(times, -1, sizeof(times));
		memset(ids, -1, sizeof(ids));
		return (0);
	}
	if (time == -1)
		flush_buffer(times, ids, strs, i);
	else
	{
		if (died)
			return (0);
		times[i] = time;
		ids[i] = id;
		strs[i] = str;
		if (str == 6)
			died = 1;
		if (i == 4095 * 5 - 1)
		{
			flush_buffer(times, ids, strs, i);
			i -= i;
		}
		else
			i++;
	}
	return (0);
}

int	flush_buffer(int *times, int *ids, int *strs, int i)
{
	int j;
	int	died_time;

	j = 0;
	if (strs[i] == 6)
		died_time = times[i];
	else
		died_time = 0;
	while(j < i)
	{
		if (times[j] == -1)
			break ;
		if (died_time != 0 && times[j] > died_time)
			j++;
		else
		{
			ft_putnbr_fd(times[j] / ms_output, 1);
			ft_putchar_fd(' ', 1);
			ft_putnbr_fd(ids[j], 1);
			if (strs[j] == 1)
				ft_putstr_fd(" has taken left fork\n", 1);
			else if (strs[j] == 2)
				ft_putstr_fd(" has taken right fork\n", 1);
			else if (strs[j] == 3)
				ft_putstr_fd(" is eating\n", 1);
			else if (strs[j] == 4)
				ft_putstr_fd(" is sleeping\n", 1);
			else if (strs[j] == 5)
				ft_putstr_fd(" is thinking\n", 1);
			else if (strs[j] == 6)
				ft_putstr_fd(" died\n", 1);
			times[j] = -1;
			ids[j] = -1;
			strs[j] = -1;
			if (strs[j] == 6)
				break ;
			j++;
		}
	}
	return (0);
}