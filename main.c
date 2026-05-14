/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 09:41:57 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/16 09:42:00 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "head.h"



int main(int argc, char **argv){
   if (argc != 9)
    {
        printf("error: wrong number of arguments\n");
        return 1;
    }
    t_args arg;
    t_data data;
    char *err;
    char *valid;
   
    valid = valid_args(argv,argc);
    if (valid ){
        printf("%s",valid);
        return 1;
    }
    arg.number_of_coders = atoi(argv[1]);
    arg.time_to_burnout = atoi(argv[2]);
    arg.time_to_compile = atoi(argv[3]);
    arg.time_to_debug = atoi(argv[4]);
    arg.time_to_refactor = atoi(argv[5]);
    arg.number_of_compiles_required = atoi(argv[6]);
    arg.dongle_cooldown = atoi(argv[7]);
    if (strcmp(argv[8],"fifo" ) == 0 )
        arg.scheduler = fifo;
    else if(strcmp(argv[8],"edf") ==0)
        arg.scheduler = edf;
    else{
        printf( "error: scheduler must be fifo or edf");
        return 1;
    }
    err = check_args(&arg);
    if (err)
    {
       printf("%s",err);
       return 1;
    }


    data.args = arg;
    data.coders = NULL;
    data.dongles = NULL;
    data.start_time = time_current();
    data.stop = 0;
    data.fifo_order = 0;
    pthread_mutex_init(&data.group_lock,NULL);
    pthread_cond_init(&data.cond_check,NULL);
    pthread_mutex_init(&data.print_lock, NULL);
    pthread_mutex_init(&data.m_stop,NULL);
    pthread_mutex_init(&data.m_last_compile,NULL);
    pthread_mutex_init(&data.compile_count,NULL);
    pthread_mutex_init(&data.dongle_valid,NULL);


    pthread_mutex_init(&data.last_active_time,NULL);

    pthread_mutex_init(&data.check_finish,NULL);

    data.stop = 0;

    data.heap = alocate_heap(data.args.number_of_coders);
    
    data.coders = create_array_coders(&data);
    data.group = 0;

    if (data.args.scheduler == fifo)
    {
         pthread_mutex_lock(&data.group_lock);

        if (data.args.number_of_coders % 2 == 0)
        {
            data.group_count_one = data.args.number_of_coders / 2;
            data.group_count_two = data.args.number_of_coders / 2;
        }
        else
        {
            data.group_count_one = (data.args.number_of_coders / 2) + 1;
            data.group_count_two = data.args.number_of_coders / 2;
        }

        pthread_mutex_unlock(&data.group_lock);

        pthread_create(&data.controller_thread, NULL, controller, &data);
    }

    data.coders->last_compile_time = data.start_time;
    data.dongles = create_array_dongles(&data);
    if (!data.coders || !data.dongles){
        return 1;
    }

    init_dongles(data.dongles,&data);
    add_dongles_to_coders(&data,data.coders,data.dongles);

    

    create_coders(&data.args, data.coders);
    


    pthread_t m_check;
    
    
    pthread_create(&m_check,NULL,monitor_check, &data);

    int i = 0;
    while (i < arg.number_of_coders)
    {
           pthread_join(data.coders[i].thread, NULL);
           i++;

    }
    
    pthread_join(data.controller_thread,NULL);
    pthread_join(m_check,NULL);

  
    int k = 0;
    while (k < arg.number_of_coders)
    {
       pthread_mutex_destroy(&data.dongles[k].mutex);
      
       k++;
    }

    pthread_mutex_destroy(&data.heap->lock);
    pthread_mutex_destroy(&data.group_lock);
    pthread_mutex_destroy(&data.print_lock);
    pthread_mutex_destroy(&data.m_stop);
    pthread_mutex_destroy(&data.m_last_compile);
    pthread_cond_destroy(&data.cond_check);
    pthread_mutex_destroy(&data.compile_count);
    pthread_mutex_destroy(&data.dongle_valid);
    pthread_mutex_destroy(&data.last_active_time);
    pthread_mutex_destroy(&data.check_finish);

    free(data.dongles);
    free(data.coders);
    
   
    free(data.heap->arr);
    free(data.heap);
    return 0;
}