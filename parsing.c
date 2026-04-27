/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 22:26:13 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/15 22:26:16 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "head.h"

char *check_args(t_args *arg){
    

    if (arg->number_of_coders< 0)
       return "error: number of coders must be positive";
    if (arg->time_to_burnout< 0)
       return "error: time to burnout must be positive";
    if (arg->time_to_compile< 0)
       return "error: time to compile must be positive";
    if (arg->time_to_debug< 0)
       return "error: time to debug must be positive";
    if (arg->time_to_refactor< 0)
       return "error: time to refector must be positive";
    if (arg->number_of_compiles_required< 0)
       return "error: number of compiles required must be positive";
    if (arg->dongle_cooldown < 0)
       return "error: dongle_cooldown must be positive";

    return NULL;
}

int is_number(char *str){

   int i;
   i = 0;

   if (!str[i])
      return 0;
   if (str[i] == '-' || str[i] == '+')
       i++;
   while (str[i])
   {
      if (str[i] < '0' || str[i] >'9')
      {
         return (0);
      }
      i++;
   }
   return 1;
}
char *valid_args(char **argv, int argc){

      int i;
      i = 1;
      while (i < argc - 1)
      {
 
         if (!is_number(argv[i]))
            return("Error : the argment should be a number ");
         i++;
         
      }
   return NULL;
}
