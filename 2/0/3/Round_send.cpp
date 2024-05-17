#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int sh_x;
pthread_mutex_t x_l;

void* func(void* argv){
    int my_num = ((int*)argv)[0];
    int yes = 0;
    while (yes == 0)
    {
        pthread_mutex_lock(&x_l);
        if (my_num == sh_x){
            printf("i:%d, num: %d\n", my_num, sh_x);
            sh_x += 1;
            yes = 1;
        }
        //printf("i: %d miss\n",  my_num);
        pthread_mutex_unlock(&x_l);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Wrong argv\n [tread num]");
        return 1;
    }
    
    int N = atoi(argv[1]);
    pthread_t arr [N];
    int arg_arr [N][1];
    sh_x = 0;

    pthread_mutex_init(&x_l, NULL);

    for(int i = 0; i < N; i++){
        arg_arr[i][0] = i;
        //printf("%d %d %d\n", i, arg_arr[i][0], arg_arr[i][1]);
        pthread_create(&arr[i], NULL, func, (void*)&arg_arr[i]);
    }


    for(int i = 0; i < N; i++){
        pthread_join(arr[i], NULL);
    }

    printf("ans %d\n", sh_x);

    return 0;
}