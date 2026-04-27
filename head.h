/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:18:22 by khnoman           #+#    #+#             */
/*   Updated: 2026/04/06 10:18:28 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
#define HEAD_H


#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>




typedef enum {
    fifo,
    edf
} scheduler_type;

typedef enum{
    compile,
    debug,
    refactor

} state_type;

typedef struct coder t_coder;

typedef struct args {
    int number_of_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    int number_of_compiles_required;
    long dongle_cooldown;
    scheduler_type scheduler;
}t_args;

typedef struct dongle{
    pthread_mutex_t mutex;
    long last_used_time;

} t_dongle;

typedef struct s_data
{
    t_args args;
    t_coder *coders;
    t_dongle *dongles;
    long start_time;
    int stop;
    pthread_mutex_t print_lock;
    pthread_mutex_t m_stop;
    pthread_mutex_t m_last_compile;

}t_data;

 typedef struct coder{
    int id;
    pthread_t thread;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    long last_compile_time;
    int compile_count;
    state_type state;
    t_data *data;
} t_coder;


char *check_args(t_args *arg);
char *valid_args(char **argv, int argc);

void create_coders(t_args *arg, t_coder *arr_coder);
t_coder *create_array_coders(t_data *data);

t_dongle *create_array_dongles(t_data *data);

void add_dongles_to_coders(t_data *data, t_coder *coder, t_dongle *dongles);

void init_dongles(t_dongle *dongles, t_data *data);


void set_stop(t_data *data);
int get_stop(t_data *data);

long long time_current(void);

void *monitor_check(void *data);

int lock_dongles(t_coder *coder);



#endif