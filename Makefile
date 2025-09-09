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

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

SRC_DIR = src
OBJ_DIR = obj

SRCS = buffer_utils.c \
		buffer.c \
		forks_lock.c \
		forks_unlock.c \
		init_philo_return.c \
		init_philo.c \
		main.c \
		monitor_utils.c \
		monitor.c \
		parsing.c \
		philo_eat.c \
		philo_sleep.c \
		philo_start.c \
		philo_think.c \
		timestamp.c \
		write_utils.c \
		writer.c

CFILES = $(foreach file, $(SRCS), $(SRC_DIR)/$(file))
OFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CFILES))

NAME = philo

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OFILES)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OFILES)

fclean: clean
	rm -rf $(OBJ_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
