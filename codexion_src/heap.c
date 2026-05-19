/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khnoman <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 10:20:32 by khnoman           #+#    #+#             */
/*   Updated: 2026/05/01 10:20:35 by khnoman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_heap	*alocate_heap(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->arr = malloc(sizeof(t_task) * capacity);
	if (!heap->arr)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = capacity;
	pthread_mutex_init(&heap->lock, NULL);
	return (heap);
}

void	heap_check(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->arr[i].priority < heap->arr[parent].priority)
		{
			swap_task(&heap->arr[i], &heap->arr[parent]);
			i = parent;
		}
		else
			break ;
	}
}

void	heap_task_down(t_heap *heap, int i)
{
	int	left_child;
	int	right_child;
	int	small;

	while (1)
	{
		right_child = (2 * i) + 2;
		left_child = (2 * i) + 1;
		small = i;
		if ((left_child < heap-> size) && (
				heap->arr[left_child].priority < heap->arr[small].priority))
			small = left_child;
		if ((right_child < heap-> size) && (
				heap->arr[right_child].priority < heap->arr[small].priority))
			small = right_child;
		if (small == i)
			break ;
		swap_task(&heap->arr[i], &heap->arr[small]);
		i = small;
	}
}

void	add_heap(t_heap *heap, t_task task)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->arr[heap->size] = task;
	heap_check(heap, heap->size);
	heap->size++;
}

t_task	remove_min(t_heap *heap)
{
	t_task	min_task;

	if (heap->size == 0)
	{
		min_task.id = -1;
		min_task.priority = -1;
		return (min_task);
	}
	min_task = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size - 1];
	heap->size--;
	heap_task_down(heap, 0);
	return (min_task);
}
