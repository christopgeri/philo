# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/31 10:01:10 by cgeringe          #+#    #+#              #
#    Updated: 2025/07/31 10:01:12 by cgeringe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCDIR = src
OBJDIR = obj

CFILES = src/main.c \
			src/init_philo.c \
			src/start_eating.c \
			src/start_philo.c \
			src/start_sleeping.c \
			src/ft_atoi.c

OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Werror -Wextra -g
CC = cc

all: $(NAME)

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OFILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

