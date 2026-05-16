/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:25:07 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/16 15:25:09 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void f_bunout(t_data *data, int *i, long long now)
{
    long long timestamp;

    set_stop(data);
    pthread_mutex_lock(&data->print_lock);
    timestamp = now - data->coders[*i].data->start_time;
    printf("%lld %d burned out\n", timestamp, data->coders[*i].id);
    pthread_mutex_unlock(&data->print_lock);
}

int check_coder_in_monitor(t_data *data)
{
    if (check_coders(data))
    {
        set_stop(data);
        return 0;
    }
    return 1;
}