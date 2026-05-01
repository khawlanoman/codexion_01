/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 10:20:32 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/01 10:20:35 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_queue *alocate_queue(){
    t_queue *queue;
    queue = malloc(sizeof(t_queue));
    if (!queue)
    {
       return NULL;
    }
    
    return queue;
}

int *create_queue_array(t_data *data){
    
    data->dongles->queue->len_queue = 2;
    data->dongles->queue->arr = malloc(2* sizeof(int));
    if (!data->dongles->queue->arr)
    {
        return NULL;
    }
    data->dongles->queue->first = 0;
    data->dongles->queue->last = -1;

    return (data->dongles->queue->arr);
}


int add_to_queue(t_data *data, int coder_id){
    
    pthread_mutex_lock(&data->dongles->queue->queue_lock);
    if (data->dongles->queue->last +1 < data->dongles->queue->len_queue)
    {
        data->dongles->queue->last++;
        data->dongles->queue->arr[data->dongles->queue->last] = coder_id;
        pthread_mutex_unlock(&data->dongles->queue->queue_lock);
        return 1;
    }
    pthread_mutex_unlock(&data->dongles->queue->queue_lock);

    return 0;


}


int first_queue_id(t_queue *queue){

    int value;
    pthread_mutex_lock(&queue->queue_lock);
    if (queue->first <= queue->last)
    {
        value = queue->arr[queue->first];
        pthread_mutex_lock(&queue->queue_lock);
        return (value);
    }
    pthread_mutex_lock(&queue->queue_lock);
    return -1;
}

int remove_first_queue_id(t_queue *queue){
     
    pthread_mutex_lock(&queue->queue_lock);
    if (queue->first > queue->last)
    {
        queue->first++;
        pthread_mutex_unlock(&queue->queue_lock);
        return 1;
    
    }
    pthread_mutex_unlock(&queue->queue_lock);
     return (0);
}
