/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h_lock_dongles.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:21:54 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/14 15:21:58 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int	for_one_dongle(t_coder *coder)
{
	long long	timestamp;
	long long	time_now;

	if (!coder || !coder->left_dongle)
		return (0);
	if (coder->data->args.number_of_coders == 1)
	{
		time_now = time_current();
		timestamp = time_now - coder->data->start_time;
		coder->first = coder->left_dongle;
		pthread_mutex_lock(&coder->left_dongle->mutex);
		pthread_mutex_lock(&coder->data->print_lock);
		printf("%lld %d has taken a dongle\n", timestamp, coder->id);
		pthread_mutex_unlock(&coder->data->print_lock);
		pthread_mutex_unlock(&coder->left_dongle->mutex);
		return (0);
	}
	return (1);
}

void	check_first_and_second(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		coder->first = coder->left_dongle;
		coder->second = coder->right_dongle;
	}
	else
	{
		coder->first = coder->right_dongle;
		coder->second = coder->left_dongle;
	}
}

void	helper_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->heap->lock);
	remove_min(coder->data->heap);
	pthread_cond_broadcast(&coder->data->cond_check);
	pthread_mutex_unlock(&coder->data->heap->lock);
	f_dongle_valid(coder);
}

void	compile_and_unlock_remove_min(t_coder *coder)
{
	if (get_stop(coder->data))
	{
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_unlock(&coder->second->mutex);
		return ;
	}
	pthread_mutex_lock(&coder->data->compile_mutex);
	coder->is_compiling = 1;
	pthread_mutex_unlock(&coder->data->compile_mutex);
	print_state(coder, "is compiling");
	if (smart_sleep(coder->data->args.time_to_compile, coder) == 1)
		return ;
	f_last_compile_time(coder);
	pthread_mutex_lock(&coder->data->compile_mutex);
	coder->is_compiling = 0;
	pthread_mutex_unlock(&coder->data->compile_mutex);
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_unlock(&coder->second->mutex);
	helper_compile(coder);
}

int	coder_cycle(t_coder *coder)
{
	t_task	task;

	fifo_groups(coder);
	task.id = coder->id;
	f_priority(coder, task);
	if (heap_check_wait(coder) == 0)
		return (0);
	if (print_and_check_dongles(coder) == 0)
		return (0);
	if (get_stop(coder->data))
	{
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_unlock(&coder->second->mutex);
		return (0);
	}
	compile_and_unlock_remove_min(coder);
	fifo_group(coder);
	debug_and_refactor(coder);
	return (1);
}
