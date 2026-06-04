/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 09:41:57 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/17 10:51:48 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int	main(int argc, char **argv)
{
	t_args		arg;
	t_data		data;
	pthread_t	m_check;

	if (argc != 9)
	{
		fprintf(stderr, "error: wrong number of arguments\n");
		return (1);
	}
	if (check_valid(argv, argc, &arg) == 1)
		return (1);
	data_init(&data, arg);
	if (data.args.scheduler == t_fifo)
		pthread_create(&data.controller_thread, NULL, controller, &data);
	data.coders->last_compile_time = data.start_time;
	data.dongles = create_array_dongles(&data);
	if (!data.coders || !data.dongles)
		return (1);
	call_functions(&data);
	pthread_create(&m_check, NULL, monitor_check, &data);
	f_join(&data, arg, m_check);
	f_destroy_and_free(&data, arg);
	return (0);
}
