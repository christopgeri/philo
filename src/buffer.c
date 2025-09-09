/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:59:20 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 13:59:23 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	buffer_new_entry_check(t_buffer *buf, t_mtxes *mtx);
int	buffer_write_thread(t_buffer *buf, t_mtxes *mtx);
int	buffer_init(t_buffer *buf);
int	buffer_change_indexes(t_buffer *buf, t_cpy_buffer *cpy);

int	buffer(long int time, int id, int str, t_mtxes *mtx)
{
	static t_buffer	buf;

	if (buf.died == 1)
		return (1);
	if (id >= 0)
	{
		buf.times[buf.y][buf.x] = time;
		buf.ids[buf.y][buf.x] = id + 1;
		buf.strs[buf.y][buf.x] = str;
		buffer_new_entry_check(&buf, mtx);
	}
	else if (id == -1)
		return (buffer_write_thread(&buf, mtx));
	else if (id == -2)
		return (buffer_init(&buf));
	else if (id == -3)
	{
		buf.died = 1;
		pthread_mutex_lock(&mtx->hold_buffer);
		buffer_flush(buf.times[buf.y], buf.ids[buf.y], buf.strs[buf.y], buf.x);
		buffer_reset(&buf, buf.y);
		pthread_mutex_unlock(&mtx->hold_buffer);
	}
	return (0);
}

int	buffer_write_thread(t_buffer *buf, t_mtxes *mtx)
{
	t_cpy_buffer	cpy;

	if (buf->x > 100)
	{
		buffer_change_indexes(buf, &cpy);
		pthread_mutex_unlock(&mtx->hold_write);
		while (cpy.x < cpy.len)
		{
			cpy.times[cpy.x] = buf->times[cpy.y][cpy.x];
			cpy.ids[cpy.x] = buf->ids[cpy.y][cpy.x];
			cpy.strs[cpy.x] = buf->strs[cpy.y][cpy.x];
			cpy.x++;
		}
		buffer_reset(buf, cpy.y);
		pthread_mutex_lock(&mtx->hold_buffer);
		buffer_flush(cpy.times, cpy.ids, cpy.strs, cpy.len);
		pthread_mutex_unlock(&mtx->hold_buffer);
		return (0);
	}
	else
		pthread_mutex_unlock(&mtx->hold_write);
	return (0);
}

int	buffer_change_indexes(t_buffer *buf, t_cpy_buffer *cpy)
{
	cpy->x = 0;
	cpy->y = buf->y;
	cpy->len = buf->x;
	if (buf->y == 0)
		buf->y++;
	else
		buf->y -= buf->y;
	buf->x -= buf->x;
	return (0);
}

int	buffer_new_entry_check(t_buffer *buf, t_mtxes *mtx)
{
	if (buf->strs[buf->y][buf->x] == 6)
	{
		buf->died = 1;
		pthread_mutex_lock(&mtx->hold_buffer);
		buffer_flush(buf->times[buf->y], buf->ids[buf->y],
			buf->strs[buf->y], buf->x + 1);
		pthread_mutex_unlock(&mtx->hold_buffer);
	}
	if (buf->x == 1023)
	{
		pthread_mutex_lock(&mtx->hold_buffer);
		buffer_flush(buf->times[buf->y], buf->ids[buf->y],
			buf->strs[buf->y], buf->x);
		buffer_reset(buf, buf->y);
		pthread_mutex_unlock(&mtx->hold_buffer);
		buf->x -= buf->x;
	}
	else
		buf->x++;
	return (0);
}

int	buffer_init(t_buffer *buf)
{
	buf->x = 0;
	buf->y = 0;
	buf->died = 0;
	buffer_reset(buf, 0);
	buffer_reset(buf, 1);
	return (0);
}
