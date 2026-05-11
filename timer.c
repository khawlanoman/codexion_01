/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:17:52 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/25 11:17:57 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int get_stop(t_data *data){
    int value;
    pthread_mutex_lock(&data->m_stop);
    value = data->stop;
    pthread_mutex_unlock(&data->m_stop);
    return value;
}

void set_stop(t_data *data){
    
    pthread_mutex_lock(&data->m_stop);
    data->stop = 1;
    pthread_mutex_unlock(&data->m_stop);

}

long long time_current(void){
    long long time_now;
    struct timeval time;
    gettimeofday(&time,NULL);
    time_now = (time.tv_sec * 1000) + (time.tv_usec / 1000);

    return time_now;
}



void *monitor_check(void *d){
     int i;
     long long now ;
     long long timestamp;
     int number_coder;
     t_data *data;
     data = (t_data *)d;
   
    number_coder = data->args.number_of_coders;
     
     while (!get_stop(data))
     {
        i = 0;
       
        now = time_current();
        while (i < number_coder)
        {
            pthread_mutex_lock(&data->m_last_compile);
           long long  x = data->coders[i].last_compile_time;
           pthread_mutex_unlock(&data->m_last_compile);
        
            if (now -  x > data->args.time_to_burnout){
                set_stop(data);
                pthread_mutex_lock(&data->print_lock);
                timestamp = now - data->coders[i].data->start_time;
                printf("%lld %d burned out\n",timestamp, data->coders[i].id);
                pthread_mutex_unlock(&data->print_lock);
                return NULL;
            }
           
           
           usleep(400);
            i++;
        }

        if (check_coders(data))
            {
                return NULL;
            }
     
        // pthread_mutex_lock(&data->monitor_lock);
        // pthread_cond_timedwait(&data->monitor_cond, &data->monitor_lock, 10);
        // pthread_mutex_unlock(&data->monitor_lock);
     }
    return NULL;
}


int check_coders(t_data *data){
    int i;

    i = 0;
    while (i < data->args.number_of_coders)
    {   pthread_mutex_lock(&data->compile_count);
        if (data->coders[i].compile_count < data->args.number_of_compiles_required){
            pthread_mutex_unlock(&data->compile_count);
            return 0;
        }
        pthread_mutex_unlock(&data->compile_count);  
        i++;
    }
    return 1;
}



void smart_sleep(long var,t_coder *coder){

   long long time_now;

   time_now = time_current();
   while (!get_stop(coder->data))
   {
    if (time_current() - time_now >= var)
            break;
     usleep(500);
   }
}

void *controller(void *arg)
{
    t_data *data;

    data = (t_data *)arg;

    while (!get_stop(data))
    {
        pthread_mutex_lock(&data->group_lock);

        if (data->group == 0 && data->group_count_one == 0)
        {
            data->group = 1;

         
            if (data->args.number_of_coders % 2 == 0)
                data->group_count_one = data->args.number_of_coders / 2;
            else
                data->group_count_one = (data->args.number_of_coders / 2) + 1;
        }

        if (data->group == 1 && data->group_count_two == 0)
        {
            data->group = 0;
            data->group_count_two = data->args.number_of_coders / 2;
        }
        pthread_mutex_unlock(&data->group_lock);
        usleep(10);
    }

    return (NULL);
}