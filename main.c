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
    //t_data data;
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

   
    return 0;
}
