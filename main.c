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


#include "head.h"

int main(int argc, char **argv)
{
     t_args arg;
    t_data data;
    char *valid;

   if (argc != 9)
    {
        printf("error: wrong number of arguments\n");
        return 1;
    }
    valid = valid_args(argv,argc);
    if (valid )
    {
        printf("%s",valid);
        return 1;
    }
    if (add_args(&arg, argv) == 1)
        return 1;
    data_init(&data, arg);
    if (data.args.scheduler == fifo)
        pthread_create(&data.controller_thread, NULL, controller, &data);
    data.coders->last_compile_time = data.start_time;
    data.dongles = create_array_dongles(&data);
    if (!data.coders || !data.dongles)
        return 1;
    // 
    call_functions(&data);
    pthread_t m_check;
    pthread_create(&m_check,NULL,monitor_check, &data);

    f_join(&data, arg, m_check);
    f_destroy_and_free(&data, arg);
    return 0;
}