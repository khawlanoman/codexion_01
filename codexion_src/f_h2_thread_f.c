/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h2_thread_f.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 18:04:38 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/17 11:01:56 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void	fifo_group(t_coder *coder)
{
	if (coder->data->args.scheduler == t_fifo)
	{
		pthread_mutex_lock(&coder->data->group_lock);
		if (coder->id % 2 == 0)
			coder->data->group_count_two--;
		else
			coder->data->group_count_one--;
		pthread_mutex_unlock(&coder->data->group_lock);
	}
}

void	debug_and_refactor(t_coder *coder)
{
	if (get_stop(coder->data))
		return ;
	print_state(coder, "is debugging");
	if (smart_sleep(coder->data->args.time_to_debug, coder) == 1)
		return ;
	if (get_stop(coder->data))
		return ;
	print_state(coder, "is refactoring");
	if (smart_sleep(coder->data->args.time_to_refactor, coder) == 1)
		return ;
}

int	print_and_check_dongles(t_coder *coder)
{
	if (lock_dongles(coder) == 0)
		return (0);
	if (get_stop(coder->data))
		return (0);
	print_state(coder, "has taken a dongle");
	print_state(coder, "has taken a dongle");
	return (1);
}

int	heap_check_wait(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->heap->lock);
	while (!get_stop(coder->data) && coder->data->heap->size == 0 )
	{
		pthread_cond_wait(&coder->data->cond_check, &coder->data->heap->lock);
	}
	if (get_stop(coder->data))
	{
		pthread_mutex_unlock(&coder->data->heap->lock);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->heap->lock);
	return (1);
}

int	check_finish(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->check_finish);
	if (coder->finish == 1)
	{
		pthread_mutex_unlock(&coder->data->check_finish);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->check_finish);
	return (1);
}
