#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct data{
    int count;
    pthread_mutex_t count_lock;
} t_data;

void *thread_f(void *arg)
{
    int i = 0;
    t_data *data = (t_data *)arg;
    // pthread_mutex_lock(&data->count_lock);
    while (i < 10000000)
    {
        data->count++;
      i++;
    }
    // pthread_mutex_unlock(&data->count_lock);
    
    
    return NULL;
}
// t_data *create_data()
// {
//     t_data *data;
//     data =malloc(sizeof(t_data));
//     if (!data)
//     {
//         return NULL;
//     }
//     return data;
// }
int main()
{
    int i = 0;
    int coder = 4;
    t_data data;
    data.count = 0;
    pthread_mutex_init(&data.count_lock,NULL);
    pthread_t thread[coder];
    printf("%d\n",data.count);
    while(i < coder)
    {
        pthread_create(&thread[i],NULL,thread_f,&data);
        i++;
    }
    i = 0;
    while (i < coder)
    {
        pthread_join(thread[i],NULL);  
        i++;
    }
    printf("%d\n",data.count);
   // free(data);
}