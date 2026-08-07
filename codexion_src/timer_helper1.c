/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_helper1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:25:07 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/17 10:56:12 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

int	check_finish_monitor(t_data *data, int *i)
{
	pthread_mutex_lock(&data->check_finish);
	if (data->coders[*i].finish == 1)
	{
		pthread_mutex_unlock(&data->check_finish);
		(*i)++;
		return (0);
	}
	pthread_mutex_unlock(&data->check_finish);
	return (1);
}

long long	read_last_active(t_data *data, int *i)
{
	long long	x;

	pthread_mutex_lock(&data->m_last_compile);
	x = data->coders[*i].last_compile_time;
	pthread_mutex_unlock(&data->m_last_compile);
	return (x);
}

void	update_group(t_data *data)
{
	pthread_mutex_lock(&data->group_lock);
	if (data->group == 0 && data->group_count_one == 0)
	{
		data->group = 1;
		if (data->args.number_of_coders % 2 == 0)
			data->group_count_one = data->args.number_of_coders / 2 ;
		else
			data->group_count_one = (data->args.number_of_coders / 2) + 1;
	}
	if (data->group == 1 && data->group_count_two == 0)
	{
		data->group = 0;
		data->group_count_two = data->args.number_of_coders / 2;
	}
	pthread_mutex_unlock(&data->group_lock);
}

int	get_stop(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->m_stop);
	value = data->stop;
	pthread_mutex_unlock(&data->m_stop);
	return (value);
}

void	set_stop(t_data *data)
{
	pthread_mutex_lock(&data->m_stop);
	data->stop = 1;
	pthread_mutex_unlock(&data->m_stop);
}
