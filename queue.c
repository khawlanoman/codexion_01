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
    
    queue->arr = malloc(sizeof(int) * 2);
    if (!queue->arr)
    {
       return NULL;
    }
    return queue;
}

int first_queue(t_queue *queue){
    return  queue->arr[0];
}


void rotate_queue_arr(t_queue *queue){

    int tmp;

    tmp = queue->arr[0];
    queue->arr[0]  = queue->arr[1];
    queue->arr[1] = tmp;
}