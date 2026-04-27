/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:17:52 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/25 11:17:57 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int get_stop(t_data *data){
    int value;
    pthread_mutex_lock(&data->m_stop);
    value = data->stop;
    pthread_mutex_unlock(&data->m_stop);

    return value;
}


long long time_current(void){
    long long time_now;
    struct timeval time;
    gettimeofday(&time,NULL);
    time_now = (time.tv_sec * 1000) + (time.tv_usec / 1000);

    return time_now;
}

