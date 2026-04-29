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
    t_coder *coder;
    long long time_now;
    long long timestamp;

    coder = (t_coder *)arg;

    while (!get_stop(coder->data) && !coder->finish)
    {
        if (coder->compile_count >= coder->data->args.number_of_compiles_required)
           {     
                coder->finish = 1;
                return NULL;
           }
      
        if (lock_dongles(coder)== 0)
        {
           
             return NULL;
        }
           

        time_now = time_current();
        timestamp = time_now - coder->data->start_time;

        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        printf("%lld %d has taken a dongle\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);
        
      
       
        //  if (get_stop(coder->data))
        //  {
        //      return NULL;
        //  }
         
        time_now = time_current();

        pthread_mutex_lock(&coder->data->m_last_compile);
        coder->last_compile_time = time_now;
        pthread_mutex_unlock(&coder->data->m_last_compile);

        timestamp = time_now - coder->data->start_time;
        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is compiling\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);
         coder->compile_count++;
        //usleep(coder->data->args.time_to_compile * 1000);
        smart_sleep(coder->data->args.time_to_compile,coder);

       pthread_mutex_unlock(&coder->right_dongle->mutex); 
       pthread_mutex_unlock(&coder->left_dongle->mutex);


       smart_sleep(coder->data->args.dongle_cooldown,coder);

        
        
        //usleep(coder->data->args.dongle_cooldown * 1000);

        // if (get_stop(coder->data))
        //     return NULL;
        
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is debuging\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        //usleep(coder->data->args.time_to_debug * 1000);
        smart_sleep(coder->data->args.time_to_debug,coder);
        
        
        // if (get_stop(coder->data))
        //     return NULL;
        
        time_now = time_current();
        timestamp = time_now - coder->data->start_time;
        pthread_mutex_lock(&coder->data->print_lock);
        printf("%lld %d is refactoring\n", timestamp, coder->id);
        pthread_mutex_unlock(&coder->data->print_lock);

        //usleep(coder->data->args.time_to_refactor * 1000);
        smart_sleep(coder->data->args.time_to_refactor,coder);

        if (get_stop(coder->data))
            return NULL;

       
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
        arr_coders[i].id = i+1;
        arr_coders[i].state = 0;
        arr_coders[i].finish =0;
        arr_coders[i].left_dongle = NULL;
        arr_coders[i].right_dongle = NULL;
        arr_coders[i].compile_count =0;
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
    if (coder->id % 2 == 0)
    {
        while (!get_stop(coder->data))
        {
            pthread_mutex_lock(&coder->right_dongle->mutex);

            if (get_stop(coder->data))
            {
                pthread_mutex_unlock(&coder->right_dongle->mutex);
                return 0;
            }

           
            pthread_mutex_lock(&coder->left_dongle->mutex);

            if (get_stop(coder->data))
            {
                pthread_mutex_unlock(&coder->right_dongle->mutex);
                pthread_mutex_unlock(&coder->left_dongle->mutex);
                return 0;
            }

            return 1;
        }
        return 0;
    }
    else
    {
        while (!get_stop(coder->data))
        {
            pthread_mutex_lock(&coder->left_dongle->mutex);

            if (get_stop(coder->data))
            {
                pthread_mutex_unlock(&coder->left_dongle->mutex);
                return 0;
            }
            
          
            pthread_mutex_lock(&coder->right_dongle->mutex);

            if (get_stop(coder->data))
            {
                pthread_mutex_unlock(&coder->left_dongle->mutex);
                pthread_mutex_unlock(&coder->right_dongle->mutex);
                return 0;
            }

            return 1;
        }
        return 0;
    }
}