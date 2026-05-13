/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_coders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:09:33 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/06 12:09:36 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

void *thread_f(void *arg)
{
    t_coder    *coder;
    t_task      task;
    long long   time_now;
    long long   timestamp;

    coder = (t_coder *)arg;

        // time_now = time_current();
        // timestamp = time_now - coder->data->start_time;
        // printf("[coder_id:%d, time:%lld]\n",coder->id,timestamp);
    pthread_mutex_lock(&coder->data->check_finish);
    while (!get_stop(coder->data) && !coder->finish)
    {
        pthread_mutex_unlock(&coder->data->check_finish);

        pthread_mutex_lock(&coder->data->last_active_time);
        coder->last_active_time = time_current();
        pthread_mutex_unlock(&coder->data->last_active_time);

        if (coder->compile_count >= coder->data->args.number_of_compiles_required)
        {
           pthread_mutex_lock(&coder->data->check_finish);
            coder->finish = 1;
           pthread_mutex_unlock(&coder->data->check_finish);
            return (NULL);
        }

        if (coder->data->args.scheduler ==fifo)
        {   pthread_mutex_lock(&coder->data->group_lock);
             if (coder->id % 2 == 0)
             {
                coder->my_group = 1;
                pthread_mutex_unlock(&coder->data->group_lock);
             }
            else{
                coder->my_group = 0;
                pthread_mutex_unlock(&coder->data->group_lock);
            }

            while (!get_stop(coder->data))
            {
                pthread_mutex_lock(&coder->data->group_lock);
                if (coder->data->group == coder->my_group)
                {
                    
                    pthread_mutex_unlock(&coder->data->group_lock);
                    break;
                }
                pthread_mutex_unlock(&coder->data->group_lock);
            }

        }  
        task.id = coder->id;

        pthread_mutex_lock(&coder->data->heap->lock);

        if (coder->data->args.scheduler == fifo)
        //i need mutex here
            task.priority = coder->data->fifo_order++;
        if (coder->data->args.scheduler == edf){
            task.priority = coder->last_compile_time + coder->data->args.time_to_burnout; 
        }
        add_heap(coder->data->heap, task);
        pthread_cond_broadcast(&coder->data->cond_check);
        pthread_mutex_unlock(&coder->data->heap->lock);


// ////
        pthread_mutex_lock(&coder->data->heap->lock);
        while (!get_stop(coder->data) && coder->data->heap->size == 0 )
        {
               pthread_cond_wait(&coder->data->cond_check, &coder->data->heap->lock);
        }

        if (get_stop(coder->data)){
            pthread_mutex_unlock(&coder->data->heap->lock);
            return (NULL);
        }
        pthread_mutex_unlock(&coder->data->heap->lock);
/////

        if (lock_dongles(coder) == 0)
            return (NULL);


        if (get_stop(coder->data))
        {
            pthread_mutex_unlock(&coder->first->mutex);
            pthread_mutex_unlock(&coder->second->mutex);
            return (NULL);
        }

        time_now = time_current();
        pthread_mutex_lock(&coder->data->print_lock);
        // printf("\nmy_group %d\n",coder->my_group);
        // printf("\ngroup %d\n",coder->data->group);
        // printf("\ngroup_count %d\n",coder->data->group_count);
         timestamp = time_now - coder->data->start_time;
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        if (get_stop(coder->data))
            return (NULL);

        pthread_mutex_lock(&coder->data->print_lock);
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        printf("%lld %d is compiling\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        // printf(" coder : %d priority: %lld , id = %d\n",coder->id ,task.priority, task.id);
       
       

        smart_sleep(coder->data->args.time_to_compile,coder );

         pthread_mutex_lock(&coder->data->m_last_compile);
        coder->last_compile_time = time_current();
        pthread_mutex_unlock(&coder->data->m_last_compile);

     
        
       
        pthread_mutex_unlock(&coder->second->mutex);
        pthread_mutex_unlock(&coder->first->mutex);


      
        pthread_mutex_lock(&coder->first->mutex);
        coder->first->is_valid = time_current() + coder->data->args.dongle_cooldown;
        coder->first->is_use = 0;
        pthread_mutex_unlock(&coder->first->mutex);

        pthread_mutex_lock(&coder->second->mutex);
        coder->second->is_valid = time_current() + coder->data->args.dongle_cooldown;
        coder->second->is_use = 0;
        pthread_mutex_unlock(&coder->second->mutex);

        pthread_mutex_lock(&coder->data->heap->lock);
        extract_min(coder->data->heap);
        pthread_mutex_unlock(&coder->data->heap->lock);

       

        if (coder->data->args.scheduler == fifo)
        {
            pthread_mutex_lock(&coder->data->group_lock);
            if (coder->id % 2 == 0)
                coder->data->group_count_two--;
            else
                coder->data->group_count_one--;
            pthread_mutex_unlock(&coder->data->group_lock);
        }
        
        if (get_stop(coder->data))
            return (NULL);

        
        pthread_mutex_lock(&coder->data->print_lock);
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        printf("%lld %d is debuging\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        smart_sleep(coder->data->args.time_to_debug,coder);
       
        if (get_stop(coder->data))
            return (NULL);

        pthread_mutex_lock(&coder->data->print_lock);
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        printf("%lld %d is refactoring\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        smart_sleep(coder->data->args.time_to_refactor,coder);

        if (get_stop(coder->data))
            return (NULL);
       
        pthread_mutex_lock(&coder->data->compile_count);
        coder->compile_count++;
        pthread_mutex_unlock(&coder->data->compile_count);
        
    }

    return (NULL);
}

t_coder *create_array_coders(t_data *data){
    int number_coders;
    t_coder *arr_coders;
    int i;

    i = 0;
    number_coders = data->args.number_of_coders;
    
    arr_coders = malloc((number_coders) * sizeof(t_coder));
    if (!arr_coders)
        return NULL;
    while (i < number_coders)
    {
        arr_coders[i].id = i + 1;
        arr_coders[i].state = 0;
        arr_coders[i].finish = 0;
        arr_coders[i].left_dongle = NULL;
        arr_coders[i].right_dongle = NULL;
        arr_coders[i].compile_count = 0;
        arr_coders[i].last_compile_time = data->start_time;
        arr_coders[i].data = data;
        i++;
    }
    
   return arr_coders;
}

void create_coders(t_args *arg, t_coder *arr_coder){

    if (arg == NULL || arg->number_of_coders <= 0)
        return ;
    int number_coder;
    int i;

    number_coder =  arg->number_of_coders;
    i = 0;

    while (i < number_coder)
   {
        pthread_create(&arr_coder[i].thread, NULL, thread_f, &arr_coder[i]);
        i++;
   }
}





int lock_dongles(t_coder *coder)
{
    long long timestamp;
    long long time_now;
    if (!coder || !coder->right_dongle || !coder->left_dongle )
        return 0;
    
    if (coder->data->args.number_of_coders == 1)
    {
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        coder->first = coder->left_dongle;
        pthread_mutex_lock(&coder->first->mutex);
        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        pthread_mutex_unlock(&coder->first->mutex);
    
        return 0;
    }

       
        if (coder->left_dongle < coder->right_dongle)
        {
            coder->first = coder->left_dongle;
            coder->second = coder->right_dongle;
        }
        else{
            coder->first = coder->right_dongle;
            coder->second = coder->left_dongle;
        }

        if (get_stop(coder->data)){
            pthread_mutex_unlock(&coder->data->dongle_valid);
            return 0;
        }
        
   
    while (!get_stop(coder->data))
    {
        
        pthread_mutex_lock(&coder->first->mutex);
        pthread_mutex_lock(&coder->second->mutex);

        time_now = time_current();
           
        if (time_now >= coder->first->is_valid  && time_now >= coder->second->is_valid && coder->first->is_use== 0 && coder->second->is_use== 0)
        {
           
           
            coder->first->is_use= 1;
            coder->second->is_use= 1;
           return 1;
            
        }
            pthread_mutex_unlock(&coder->first->mutex);
            pthread_mutex_unlock(&coder->second->mutex);
            usleep(1000);
        
    }

    return 1;
}