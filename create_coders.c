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

    while (!get_stop(coder->data) && !coder->finish)
    {
        if (coder->compile_count >= coder->data->args.number_of_compiles_required)
        {
            coder->finish = 1;
            return (NULL);
        }

        if (coder->id % 2 == 0)
        {
            coder->my_group = 1;
          
        }
        else
            coder->my_group = 0;

        while (!get_stop(coder->data))
        {
            pthread_mutex_lock(&coder->data->group_lock);
            if (coder->data->group == coder->my_group)
            {
                 
                pthread_mutex_unlock(&coder->data->group_lock);
                break;
            }
            pthread_mutex_unlock(&coder->data->group_lock);
            usleep(500);
        }



        task.id = coder->id;

        pthread_mutex_lock(&coder->data->heap->lock);

        if (coder->data->args.scheduler == fifo)
            task.priority = coder->data->fifo_order++;
        if (coder->data->args.scheduler == edf)
            task.priority = coder->last_compile_time + coder->data->args.time_to_burnout; 

        add_heap(coder->data->heap, task);

        pthread_mutex_unlock(&coder->data->heap->lock);


        while (!get_stop(coder->data))
        {
            pthread_mutex_lock(&coder->data->heap->lock);

            if (coder->data->heap->size > 0 )
            {
                pthread_mutex_unlock(&coder->data->heap->lock);
                break;
            }

            pthread_mutex_unlock(&coder->data->heap->lock);
            usleep(50);
        }

        if (get_stop(coder->data))
            return (NULL);

        if (lock_dongles(coder) == 0)
            return (NULL);

        time_now = time_current();
        timestamp = time_now - coder->data->start_time;

        pthread_mutex_lock(&coder->data->print_lock);
        // printf("\nmy_group %d\n",coder->my_group);
        // printf("\ngroup %d\n",coder->data->group);
        // printf("\ngroup_count %d\n",coder->data->group_count);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        if (get_stop(coder->data))
            return (NULL);

        
        time_now = time_current();
        pthread_mutex_lock(&coder->data->m_last_compile);
        coder->last_compile_time = time_now;
        pthread_mutex_unlock(&coder->data->m_last_compile);

      
        timestamp = time_now - coder->data->start_time;
        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is compiling\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        coder->compile_count++;

        smart_sleep(coder->data->args.time_to_compile,coder );


        pthread_mutex_unlock(&coder->second->mutex);
        pthread_mutex_unlock(&coder->first->mutex);

      

        pthread_mutex_lock(&coder->data->heap->lock);
        extract_min(coder->data->heap);
        pthread_mutex_unlock(&coder->data->heap->lock);

      

        pthread_mutex_lock(&coder->data->group_lock);
        if (coder->id % 2 == 0)
            coder->data->group_count_two--;
        else
            coder->data->group_count_one--;
        pthread_mutex_unlock(&coder->data->group_lock);

        smart_sleep(coder->data->args.dongle_cooldown,coder);

        if (get_stop(coder->data))
            return (NULL);

        

        time_now = time_current();
        timestamp = time_now - coder->data->start_time;

        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is debuging\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        smart_sleep(coder->data->args.time_to_debug,coder);

        if (get_stop(coder->data))
            return (NULL);

       

        time_now = time_current();
        timestamp = time_now - coder->data->start_time;

        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is refactoring\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        smart_sleep(coder->data->args.time_to_refactor,coder);

        if (get_stop(coder->data))
            return (NULL);
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
    if (!arr_coders){
        return NULL;
    }
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

    if (arg == NULL || arg->number_of_coders <= 0){
        return ;
    }
    int number_coder;
   

    number_coder =  arg->number_of_coders;
    int i;
    i = 0;
 
    while (i < number_coder)
   {
        pthread_create(&arr_coder[i].thread, NULL, thread_f, &arr_coder[i]);
        i++;
   }
}


int lock_dongles(t_coder *coder)
{
    if (!coder || !coder->left_dongle || !coder->right_dongle)
    {
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

    pthread_mutex_lock(&coder->first->mutex);
    pthread_mutex_lock(&coder->second->mutex);

    return 1;
}