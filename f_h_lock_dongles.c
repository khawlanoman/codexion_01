/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h_lock_dongles.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:21:54 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/14 15:21:58 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int for_one_dongle(t_coder *coder)
{
    long long timestamp;
    long long time_now;
    if (!coder || !coder->left_dongle)
        return 0;
    if (coder->data->args.number_of_coders == 1)
    {
       time_now = time_current();
       timestamp = time_now - coder->data->start_time;
       coder->first = coder->left_dongle;
       pthread_mutex_lock(&coder->left_dongle->mutex);
       pthread_mutex_lock(&coder->data->print_lock);
       printf("%lld %d has taken a dongle\n",timestamp, coder->id);
       pthread_mutex_unlock(&coder->data->print_lock);
       pthread_mutex_unlock(&coder->left_dongle->mutex);
       return 0;
    }
    
    return 1;
}

void check_first_and_second(t_coder *coder)
{
    if (coder->left_dongle < coder->right_dongle)
    {
        coder->first = coder->left_dongle;
        coder->second = coder->right_dongle;
    }
    else{
        coder->first = coder->right_dongle;
        coder->second = coder->left_dongle;
    }
    
}