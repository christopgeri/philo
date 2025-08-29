#include "../philo.h"

int	write_status(int time, int id, char *str)
{
	ft_putnbr_fd(time / ms_output, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(id, 1);
	ft_putstr_fd(str, 1);
	pthread_mutex_unlock(&hold_write);
	return (1);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;
	int	length;
	
	i = 0;
	length = ft_strlen(s);
	write(fd, s, length);
	// while (s[i] != '\0')
	// {
		// ft_putchar_fd(s[i], fd);
		// i++;
	// }
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + '0', fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}