/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 17:39:25 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/19 11:46:20 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

int	add_args(t_args *arg, char **argv)
{
	char	*err;

	arg->number_of_coders = atoi(argv[1]);
	arg->time_to_burnout = atoi(argv[2]);
	arg->time_to_compile = atoi(argv[3]);
	arg->time_to_debug = atoi(argv[4]);
	arg->time_to_refactor = atoi(argv[5]);
	arg->number_of_compiles_required = atoi(argv[6]);
	arg->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo" ) == 0)
		arg->scheduler = t_fifo;
	else if (strcmp(argv[8], "edf") == 0)
		arg->scheduler = t_edf;
	else
	{
		printf("error: scheduler must be fifo or edf");
		return (1);
	}
	err = check_args(arg);
	if (err)
	{
		fprintf(stderr, "%s", err);
		return (1);
	}
	return (0);
}

void	data_init(t_data *data, t_args arg)
{
	data->args = arg;
	data->coders = NULL;
	data->dongles = NULL;
	data->start_time = time_current();
	data->stop = 0;
	data->fifo_order = 0;
	data->group = 0;
	data->group_count_one = 0;
	data->group_count_two = 0;
	pthread_mutex_init(&data->group_lock, NULL);
	pthread_cond_init(&data->cond_check, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->m_last_compile, NULL);
	pthread_mutex_init(&data->compile_count, NULL);
	pthread_mutex_init(&data->last_active_time, NULL);
	pthread_mutex_init(&data->check_finish, NULL);
	data->heap = alocate_heap(data->args.number_of_coders);
	data->coders = create_array_coders(data);
}

void	f_join(t_data *data, t_args arg, pthread_t m_check)
{
	int	i;

	i = 0;
	while (i < arg.number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	if (data->args.scheduler == t_fifo)
		pthread_join(data->controller_thread, NULL);
	pthread_join(m_check, NULL);
}

void	f_destroy_and_free(t_data *data, t_args arg)
{
	int	k;

	k = 0;
	while (k < arg.number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[k].mutex);
		k++;
	}
	pthread_mutex_destroy(&data->heap->lock);
	pthread_mutex_destroy(&data->group_lock);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->m_stop);
	pthread_mutex_destroy(&data->m_last_compile);
	pthread_cond_destroy(&data->cond_check);
	pthread_mutex_destroy(&data->compile_count);
	pthread_mutex_destroy(&data->last_active_time);
	pthread_mutex_destroy(&data->check_finish);
	free(data->dongles);
	free(data->coders);
	free(data->heap->arr);
	free(data->heap);
}

void	call_functions(t_data *data)
{
	init_dongles(data->dongles, data);
	add_dongles_to_coders(data, data->coders, data->dongles);
	create_coders(&data->args, data->coders);
}
