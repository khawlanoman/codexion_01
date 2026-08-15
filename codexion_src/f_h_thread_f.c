/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h_thread_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:44:18 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/17 11:10:34 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void	print_state(t_coder *coder, char *string)
{
	long long	timestamp;
	long long	time_now;

	pthread_mutex_lock(&coder->data->print_lock);
	if (!get_stop(coder->data))
	{
		time_now = time_current();
		timestamp = time_now - coder->data->start_time;
		printf("%lld %d %s\n", timestamp, coder->id, string);
	}
	pthread_mutex_unlock(&coder->data->print_lock);
}

void	f_last_compile_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->m_last_compile);
	coder->last_compile_time = time_current();
	pthread_mutex_unlock(&coder->data->m_last_compile);
}

void	f_dongle_valid(t_coder *coder)
{
	if (!get_stop(coder->data))
	{
		pthread_mutex_lock(&coder->first->mutex);
		coder->first->is_valid = time_current()
			+ coder->data->args.dongle_cooldown;
		coder->first->is_use = 0;
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_lock(&coder->second->mutex);
		coder->second->is_valid = time_current()
			+ coder->data->args.dongle_cooldown;
		coder->second->is_use = 0;
		pthread_mutex_unlock(&coder->second->mutex);
	}
}

void	fifo_groups(t_coder *coder)
{
	if (coder->data->args.scheduler == t_fifo)
	{
		pthread_mutex_lock(&coder->data->group_lock);
		if (coder->id % 2 == 0)
			coder->my_group = 1;
		else
			coder->my_group = 0;
		pthread_mutex_unlock(&coder->data->group_lock);
		while (!get_stop(coder->data))
		{
			pthread_mutex_lock(&coder->data->group_lock);
			if (coder->data->group == coder->my_group
				&& coder->compile_count == coder->data->phase)
			{
				pthread_mutex_unlock(&coder->data->group_lock);
				break ;
			}
			pthread_mutex_unlock(&coder->data->group_lock);
			usleep(100);
		}
	}
}

void	f_priority(t_coder *coder, t_task task)
{
	pthread_mutex_lock(&coder->data->heap->lock);
	if (coder->data->args.scheduler == t_fifo)
		task.priority = coder->data->fifo_order++;
	else if (coder->data->args.scheduler == t_edf)
		task.priority = coder->last_compile_time
			+ coder->data->args.time_to_burnout;
	add_heap(coder->data->heap, task);
	pthread_cond_broadcast(&coder->data->cond_check);
	pthread_mutex_unlock(&coder->data->heap->lock);
}
