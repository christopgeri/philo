#include "../philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	num;
	int	countminus;

	num = 0;
	i = 0;
	countminus = 1;
	while (nptr[i] == ' ' || nptr[i] == '\f' || nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\t' || nptr[i] == '\v')
		i++;
	if (nptr[i] == '-')
		countminus = -1;
	if (nptr[i] == '-' || nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + (nptr[i] - 48);
		i++;
	}
	return (num * countminus);
}

// void	ft_putchar_fd(char c, int fd)
// {
// 	write(fd, &c, 1);
// }

// void	ft_putnbr_fd(int n, int fd)
// {
// 	if (n == -2147483648)
// 	{
// 		write(fd, "-2147483648", 11);
// 		return ;
// 	}
// 	if (n < 0)
// 	{
// 		ft_putchar_fd('-', fd);
// 		n *= -1;
// 	}
// 	if (n > 9)
// 	{
// 		ft_putnbr_fd(n / 10, fd);
// 		ft_putchar_fd(n % 10 + '0', fd);
// 	}
// 	else
// 		ft_putchar_fd(n + '0', fd);
// }