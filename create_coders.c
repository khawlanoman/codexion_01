/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_coders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:09:33 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/16 16:06:28 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void	*thread_f(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_stop(coder->data))
	{
		if (check_finish(coder) == 0)
			break ;
		check_last_active(coder);
		if (check_compile_count(coder) == 0)
			return (NULL);
		if (coder_cycle(coder) == 0)
			return (NULL);
		if (get_stop(coder->data))
			return (NULL);
		pthread_mutex_lock(&coder->data->compile_count);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->data->compile_count);
	}
	return (NULL);
}

t_coder	*create_array_coders(t_data *data)
{
	int		number_coders;
	int		i;
	t_coder	*arr_coders;

	i = 0;
	number_coders = data->args.number_of_coders;
	arr_coders = malloc((number_coders) * sizeof(t_coder));
	if (!arr_coders)
		return (NULL);
	while (i < number_coders)
	{
		arr_coders[i].id = i + 1;
		arr_coders[i].finish = 0;
		arr_coders[i].left_dongle = NULL;
		arr_coders[i].right_dongle = NULL;
		arr_coders[i].compile_count = 0;
		arr_coders[i].last_compile_time = data->start_time;
		arr_coders[i].data = data;
		i++;
	}
	return (arr_coders);
}

void	create_coders(t_args *arg, t_coder *arr_coder)
{
	int	number_coder;
	int	i;

	i = 0;
	if (arg == NULL || arg->number_of_coders <= 0)
		return ;
	number_coder = arg->number_of_coders;
	while (i < number_coder)
	{
		pthread_create(&arr_coder[i].thread, NULL, thread_f, &arr_coder[i]);
		i++;
	}
}

int	lock_dongles(t_coder *coder)
{
	long long	time_now;

	if (!coder || !coder->right_dongle || !coder->left_dongle)
		return (0);
	if (for_one_dongle(coder) == 0)
		return (0);
	check_first_and_second(coder);
	while (!get_stop(coder->data))
	{
		pthread_mutex_lock(&coder->first->mutex);
		pthread_mutex_lock(&coder->second->mutex);
		time_now = time_current();
		if (time_now >= coder->first->is_valid
			&& time_now >= coder->second->is_valid
			&& coder->first->is_use == 0 && coder->second->is_use == 0)
		{
			coder->first->is_use = 1;
			coder->second->is_use = 1;
			return (1);
		}
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_unlock(&coder->second->mutex);
		usleep(1000);
	}
	return (1);
}
