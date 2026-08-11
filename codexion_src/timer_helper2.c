/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_helper2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:25:07 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/17 11:25:15 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void	f_bunout(t_data *data, int *i, long long now)
{
	long long	timestamp;

	set_stop(data);
	pthread_mutex_lock(&data->print_lock);
	timestamp = now - data->coders[*i].data->start_time;
	printf("%lld %d burned out\n", timestamp, data->coders[*i].id);
	pthread_mutex_unlock(&data->print_lock);
}

int	check_coder_in_monitor(t_data *data)
{
	if (check_coders(data))
	{
		set_stop(data);
		return (0);
	}
	return (1);
}

int	check_compile_count(t_coder *coder)
{
	//pthread_mutex_lock(&coder->data->compile_count);
	if (coder->compile_count >= coder->data->args.number_of_compiles_required)
	{
		pthread_mutex_lock(&coder->data->check_finish);
		coder->finish = 1;
		pthread_mutex_unlock(&coder->data->check_finish);
		//pthread_mutex_unlock(&coder->data->compile_count);
		return (0);
	}
	//pthread_mutex_unlock(&coder->data->compile_count);
	return (1);
}

int	check_valid(char **argv, int argc, t_args *arg)
{
	char	*valid;

	valid = valid_args(argv, argc);
	if (valid)
	{
		fprintf(stderr, "%s", valid);
		return (1);
	}
	if (add_args(arg, argv) == 1)
		return (1);
	return (0);
}
