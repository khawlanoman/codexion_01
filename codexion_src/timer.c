/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:17:52 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/16 16:56:13 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

long long	time_current(void)
{
	long long		time_now;
	struct timeval	time;

	gettimeofday(&time, NULL);
	time_now = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (time_now);
}

void	*monitor_check(void *d)
{
	int			i;
	long long	now;
	t_data		*data;

	data = (t_data *)d;
	while (!get_stop(data))
	{
		i = 0;
		now = time_current();
		while (!check_compile(data, &i) && i < data->args.number_of_coders)
		{
			if (check_finish_monitor(data, &i) == 0)
				continue ;
			if (now - read_last_active(data, &i) > data->args.time_to_burnout)
			{
				f_bunout(data, &i, now);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
		if (check_coder_in_monitor(data) == 0)
			return (NULL);
	}
	return (NULL);
}

int	check_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		pthread_mutex_lock(&data->compile_count);
		if (data->coders[i].compile_count
			< data->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->compile_count);
			return (0);
		}
		pthread_mutex_unlock(&data->compile_count);
		i++;
	}
	return (1);
}

int	smart_sleep(long var, t_coder *coder)
{
	long long	start_time;
	long long	time_left;
	long long	last_update;

	start_time = time_current();
	time_left = 0;
	last_update = start_time;
	while ((!get_stop(coder->data) && time_left < var))
	{
		usleep(100);
		time_left = time_current() - start_time;
		if (time_left >= var)
			break ;
		if (get_stop(coder->data))
			break ;
		if (time_current() - last_update >= 10)
		{
			last_update = time_current();
		}
	}
	return (0);
}

void	*controller(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!get_stop(data))
	{
		update_group(data);
		usleep(10);
	}
	return (NULL);
}
