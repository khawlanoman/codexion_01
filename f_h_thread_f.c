/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h_thread_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:44:18 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/14 15:44:21 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"



void print_state(t_coder *coder, char *string)
{
    long long timestamp;
    long long time_now;

    time_now = time_current();
    pthread_mutex_lock(&coder->data->print_lock);
    timestamp = time_now - coder->data->start_time;
    printf("%lld %d %s\n",timestamp,coder->id,string);
    pthread_mutex_unlock(&coder->data->print_lock);
}

// void f_compile_count(t_coder *coder)
// {
//     pthread_mutex_lock(&coder->data->compile_count);
//     coder->compile_count++;
//     pthread_mutex_unlock(&coder->data->compile_count);
// }

// void f_last_compile_time(t_coder *coder)
// {
//     pthread_mutex_lock(&coder->data->m_last_compile);
//     coder->last_compile_time = time_current();
//     pthread_mutex_lock(&coder->data->m_last_compile);
// }