/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_dongels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:37:16 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/17 14:37:18 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

t_dongle	*create_array_dongles(t_data *data)
{
	int			number_dongles;
	t_dongle	*arr_dongles;

	if (data == NULL || data->args.number_of_coders <= 0)
		return (NULL);
	number_dongles = data->args.number_of_coders;
	arr_dongles = malloc((number_dongles) * sizeof(t_dongle));
	if (!arr_dongles)
		return (NULL);
	return (arr_dongles);
}

void	init_dongles(t_dongle *dongles, t_data *data)
{
	int	number_dongles;
	int	i;

	i = 0;
	number_dongles = data->args.number_of_coders;
	while (i < number_dongles)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		dongles[i].is_valid = 0;
		dongles[i].is_use = 0;
		i++;
	}
}

void	add_dongles_to_coders(t_data *data, t_coder *coder, t_dongle *dongles)
{
	int	number_coder;
	int	i;

	if (data == NULL || coder == NULL || dongles == NULL
		|| data->args.number_of_coders <= 0)
		return ;
	number_coder = data->args.number_of_coders;
	i = 0;
	while (i < number_coder)
	{
		coder[i].left_dongle = &dongles[i];
		coder[i].right_dongle = &dongles[(i +1) % number_coder];
		i++;
	}
}
