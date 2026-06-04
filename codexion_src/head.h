/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 10:18:22 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/19 12:26:01 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include <time.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum scheduler
{
	t_fifo,
	t_edf
}	t_scheduler_type;
typedef struct coder	t_coder;
typedef struct args
{
	int					number_of_coders;
	int					number_of_compiles_required;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	long				dongle_cooldown;
	t_scheduler_type	scheduler;
}	t_args;
typedef struct s_task
{
	int			id;
	long long	priority;
}	t_task;
typedef struct s_heap
{
	int				size;
	int				capacity;
	t_task			*arr;
	pthread_mutex_t	lock;
}	t_heap;
typedef struct dongle
{
	int				is_use;
	long long		is_valid;
	pthread_mutex_t	mutex;
}	t_dongle;
typedef struct s_data
{
	int				stop;
	int				fifo_order;
	int				group;
	int				group_count_one;
	int				group_count_two;
	long			start_time;
	t_args			args;
	t_heap			*heap;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		controller_thread;
	pthread_cond_t	cond_check;
	pthread_mutex_t	group_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	m_stop;
	pthread_mutex_t	m_last_compile;
	pthread_mutex_t	compile_count;
	pthread_mutex_t	last_active_time;
	pthread_mutex_t	check_finish;
}	t_data;
typedef struct coder
{
	int			id;
	int			compile_count;
	int			finish;
	int			my_group;
	long		last_compile_time;
	long		last_active_time;
	t_data		*data;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_dongle	*first;
	t_dongle	*second;
	pthread_t	thread;
}	t_coder;
int				get_stop(t_data *data);
int				check_coders(t_data *data);
int				lock_dongles(t_coder *coder);
int				print_and_check_dongles(t_coder *coder);
int				heap_check_wait(t_coder *coder);
int				check_finish(t_coder *coder);
int				for_one_dongle(t_coder *coder);
int				coder_cycle(t_coder *coder);
int				check_compile_count(t_coder *coder);
int				check_finish_monitor(t_data *data, int *i);
int				check_coder_in_monitor(t_data *data);
int				add_args(t_args *arg, char **argv);
int				check_valid(char **argv, int argc, t_args *arg);
char			*check_args(t_args *arg);
char			*valid_args(char **argv, int argc);
void			create_coders(t_args *arg, t_coder *arr_coder);
void			add_dongles_to_coders(t_data *data,
					t_coder *coder, t_dongle *dongles);
void			init_dongles(t_dongle *dongles, t_data *data);
void			set_stop(t_data *data);
void			*monitor_check(void *data);
void			smart_sleep(long var, t_coder *coder);
void			add_heap(t_heap *heap, t_task task);
void			swap_task(t_task *a, t_task *b);
void			heap_check(t_heap *heap, int i);
void			heap_task_down(t_heap *heap, int i);
void			check_first_and_second(t_coder *coder);
void			print_state(t_coder *coder, char *string);
void			f_last_compile_time(t_coder *coder);
void			f_dongle_valid(t_coder *coder);
void			fifo_groups(t_coder *coder);
void			f_priority(t_coder *coder, t_task task);
void			fifo_group(t_coder *coder);
void			debug_and_refactor(t_coder *coder);
void			compile_and_unlock_remove_min(t_coder *coder);
void			check_last_active(t_coder *coder);
void			*controller(void *arg);
void			update_group(t_data *data);
void			f_bunout(t_data *data, int *i, long long now);
void			data_init(t_data *data, t_args arg);
void			f_destroy_and_free(t_data *data, t_args arg);
void			f_join(t_data *data, t_args arg, pthread_t m_check);
void			call_functions(t_data *data);
t_coder			*create_array_coders(t_data *data);
t_dongle		*create_array_dongles(t_data *data);
t_heap			*alocate_heap(int capacity);
t_task			remove_min(t_heap *heap);
long long		time_current(void);
long long		read_last_active(t_data *data, int *i);

#endif
