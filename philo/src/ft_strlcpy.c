/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:04:12 by cgeringe          #+#    #+#             */
/*   Updated: 2024/10/11 22:20:01 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	j;

	j = 0;
	if (size > 0)
	{
		while (src[j] != '\0' && j < (size - 1))
		{
			dst[j] = src[j];
			j++;
		}
		dst[j] = '\0';
	}
	return (ft_strlen(src));
}
// #include <stdio.h>
// // #include <bsd/string.h>
// #include <string.h>
// int main(void)
// {
// 	char	dst[] = "hello";
// 	char	src[] = "good morning";
// 	printf("%s\n", dst);
// 	printf("%zu\n", ft_strlcpy(dst, src, 2));
// 	printf("%s\n", dst);
// 	printf("%zu\n", ft_strlcpy(dst, src, 0));
// 	printf("%s\n", dst);

// 	char	dst2[] = "hello";
// 	char	src2[] = "good morning";
// 	printf("%s\n", dst2);
// 	printf("%zu\n", strlcpy(dst2, src2, 2));
// 	printf("%s\n", dst2);
// 	return (0);
// 	// compiling with -lbsd
// }