#include "head.h"

t_coder *create_array_coders(t_data *data){
    t_coder *arr_coder;
    int number_coders;
    int i;

    number_coders= data->args.number_of_coders;
    i = 0;
    arr_coder =malloc(number_coders * sizeof(t_coder));
    if (!arr_coder)
    {
        return NULL;
    }
    while (i < number_coders)
    {
        arr_coder[i].id = i +1;
        arr_coder[i].data =data;
        arr_coder[i].last_compile_time = 0;
        arr_coder[i].left_dongle = NULL;
        arr_coder[i].right_dongle = NULL;
        i++;
    }
    return NULL;
}

void *thread_f(void *arg){
    t_coder *coder;

    coder = (void *)arg;

    while (get_stop(coder->data))
    {
        if ()
        {
            /* code */
        }
        
    }
    
    return NULL;

}

void create_coders(t_args *arg, t_coder *array_coders){

    int i;
    int number_coders;

    i =0;
    number_coders =arg->number_of_coders;

    while (i < number_coders)
    {
        pthread_create(&array_coders[i].thread,NULL,thread_f,&array_coders[i]);
        i++;
    }

}