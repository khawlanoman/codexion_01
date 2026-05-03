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
    t_coder *coder = (t_coder *)arg;
    t_task task;
    long long timestamp;

    while (!get_stop(coder->data) && !coder->finish)
    {
        // stop condition
        if (coder->compile_count >= coder->data->args.number_of_compiles_required)
        {
            coder->finish = 1;
            return NULL;
        }


        //  create task
        task.id = coder->id;

        if (coder->data->args.scheduler == fifo)
            task.priority = coder->id;  
        //else
         //   task.priority = coder->deadline;       

        
        // add heap

        pthread_mutex_lock(&coder->data->heap->lock);
        add_heap(coder->data->heap, task);
        pthread_mutex_unlock(&coder->data->heap->lock);

       
       //check and wait
        while (!get_stop(coder->data))
        {
            pthread_mutex_lock(&coder->data->heap->lock);

            if (coder->data->heap->arr[0].id == coder->id)
            {
                pthread_mutex_unlock(&coder->data->heap->lock);
                break;
            }

            pthread_mutex_unlock(&coder->data->heap->lock);
            usleep(500);
        }

        
        if (lock_dongles(coder) == 0)
            return NULL;

        timestamp = time_current() - coder->data->start_time;
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);

        
        coder->last_compile_time = time_current();

        timestamp = time_current() - coder->data->start_time;
        printf("%lld %d is compiling\n", timestamp, coder->id);

        coder->compile_count++;

        smart_sleep(coder->data->args.time_to_compile, coder);

        
        pthread_mutex_unlock(&coder->right_dongle->mutex);
        pthread_mutex_unlock(&coder->left_dongle->mutex);


        //  remove form heap

        pthread_mutex_lock(&coder->data->heap->lock);
        extract_min(coder->data->heap);
        pthread_mutex_unlock(&coder->data->heap->lock);

       
        smart_sleep(coder->data->args.time_to_debug, coder);

        if (get_stop(coder->data))
            return NULL;

        timestamp = time_current() - coder->data->start_time;
        printf("%lld %d is debugging\n", timestamp, coder->id);

      
        smart_sleep(coder->data->args.time_to_refactor, coder);

        if (get_stop(coder->data))
            return NULL;

        timestamp = time_current() - coder->data->start_time;
        printf("%lld %d is refactoring\n", timestamp, coder->id);
    }

    return NULL;
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

    // if (coder->id % 2 == 0)
    // {
    //     while (!get_stop(coder->data))
    //     {
    //         pthread_mutex_lock(&coder->right_dongle->mutex);

    //         if (get_stop(coder->data))
    //         {
    //             pthread_mutex_unlock(&coder->right_dongle->mutex);
    //             return 0;
    //         }

           
    //         pthread_mutex_lock(&coder->left_dongle->mutex);

    //         if (get_stop(coder->data))
    //         {
    //             pthread_mutex_unlock(&coder->right_dongle->mutex);
    //             pthread_mutex_unlock(&coder->left_dongle->mutex);
    //             return 0;
    //         }

    //         return 1;
    //     }
    //     return 0;
    // }
    // else
    // {
    //     while (!get_stop(coder->data))
    //     {
    //         pthread_mutex_lock(&coder->left_dongle->mutex);

    //         if (get_stop(coder->data))
    //         {
    //             pthread_mutex_unlock(&coder->left_dongle->mutex);
    //             return 0;
    //         }
            
          
    //         pthread_mutex_lock(&coder->right_dongle->mutex);

    //         if (get_stop(coder->data))
    //         {
    //             pthread_mutex_unlock(&coder->left_dongle->mutex);
    //             pthread_mutex_unlock(&coder->right_dongle->mutex);
    //             return 0;
    //         }

    //         return 1;
    //     }
    //     return 0;
    // }

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