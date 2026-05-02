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

t_dongle *create_array_dongles(t_data *data){
    if (data == NULL || data->args.number_of_coders <= 0){
        return NULL;
    }

    int number_dongles;
    t_dongle *arr_dongles;

    number_dongles = data->args.number_of_coders;
    arr_dongles = malloc((number_dongles) * sizeof(t_dongle));
    if (!arr_dongles)
    {
        return NULL;
    }

    return arr_dongles;
}



void init_dongles(t_dongle *dongles, t_data *data){

    int number_dongles;
    int i;

    i = 0;
    number_dongles = data->args.number_of_coders;

     while (i < number_dongles)
    {
       pthread_mutex_init(&dongles[i].mutex, NULL);

       if (data->args.scheduler == fifo)
       {
            dongles[i].queue = alocate_queue();
            pthread_mutex_init(&dongles[i].queue->queue_lock, NULL);

            dongles[i].queue->arr[0] = i + 1;

            if (i == 0)
                dongles[i].queue->arr[1] = number_dongles;
            else
                dongles[i].queue->arr[1] =  i;
       }
    //    printf("\n arr:%d, %d\n", dongles[i].queue->arr[0], dongles[i].queue->arr[1]);
       i++;
    } 
    
}


void add_dongles_to_coders(t_data *data, t_coder *coder, t_dongle *dongles){

    if (data == NULL || coder== NULL || dongles ==NULL || data->args.number_of_coders <= 0){
        return ; 
    }
    int number_coder;
    int i;

    number_coder = data->args.number_of_coders;
    
    i = 0;

    while(i < number_coder){
        coder[i].left_dongle = &dongles[i];
        coder[i].right_dongle = &dongles[(i +1) % number_coder];
        
        i++;
    }
}



