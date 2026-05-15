/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_h2_thread_f.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 18:04:38 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/14 18:04:40 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void fifo_group(t_coder *coder)
{
    if (coder->data->args.scheduler == fifo)
    {
        pthread_mutex_lock(&coder->data->group_lock);
        if (coder->id % 2 == 0)
            coder->data->group_count_two--;
        else
            coder->data->group_count_one--;
        pthread_mutex_unlock(&coder->data->group_lock);
    }
    
}

void debug_and_refactor(t_coder *coder)
{
    if (get_stop(coder->data))
        return ;
    print_state(coder, "is debuging");
    smart_sleep(coder->data->args.time_to_debug,coder);
    f_last_compile_time(coder);

    if (get_stop(coder->data))
        return;
    print_state(coder, "is refactoring");
    smart_sleep(coder->data->args.time_to_refactor,coder);
    f_last_compile_time(coder);

}

int print_and_check_dongles(t_coder *coder)
{
    if (lock_dongles(coder) == 0)
       return 0; 
    
    if (get_stop(coder->data))
    {
        pthread_mutex_unlock(&coder->first->mutex);
        pthread_mutex_unlock(&coder->second->mutex);
        return 0;
    }
    print_state(coder, "has taken a dongle");
    print_state(coder, "has taken a dongle"); 
    return 1;   
}


int  heap_check_wait(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->heap->lock);
    while (!get_stop(coder->data) && coder->data->heap->size == 0)
    {
        pthread_cond_wait(&coder->data->cond_check,&coder->data->heap->lock);
    }
    if (get_stop(coder->data))
    {
        pthread_mutex_unlock(&coder->data->heap->lock);
        return 0;
    }
    pthread_mutex_unlock(&coder->data->heap->lock);
    return 1;
}

int check_finish(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->check_finish);
    if (coder->finish == 1)
    {
        pthread_mutex_unlock(&coder->data->check_finish);
        return 0;
    }
    pthread_mutex_unlock(&coder->data->check_finish);
    return 1;
}

void compile_and_unlock_remove_min(t_coder *coder)
{
    print_state(coder, "is compiling");
    smart_sleep(coder->data->args.time_to_compile,coder);
    f_last_compile_time(coder);

    pthread_mutex_unlock(&coder->second->mutex);
    pthread_mutex_unlock(&coder->first->mutex);

    f_dongle_valid(coder);
    pthread_mutex_lock(&coder->data->heap->lock);
    remove_min(coder->data->heap);
    pthread_mutex_unlock(&coder->data->heap->lock);
}

int coder_cycle(t_coder *coder)
{
    t_task task;

    fifo_groups(coder);
    task.id = coder->id;
    f_priority(coder, task);
    if (heap_check_wait(coder) == 0)
        return 0;
    if (print_and_check_dongles(coder) == 0)
        return 0;
    if (get_stop(coder->data))
        return 0;
    compile_and_unlock_remove_min(coder);
    fifo_group(coder);
    debug_and_refactor(coder);
    return 1;
}