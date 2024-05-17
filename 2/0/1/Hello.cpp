#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

void* func(void* argv){
    printf("I am %d of %d\n", ((int*)argv)[0], ((int*)argv)[1]);
    pthread_exit(NULL);
}

int main(){
    int N = 4;
    pthread_t arr [4];
    int arg_arr [N][2];

    for(int i = 0; i < N; i++){
        arg_arr[i][0] = i;
        arg_arr[i][1] = N;
        pthread_create(&arr[i], NULL, func, (void*)&arg_arr[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(arr[i], NULL);
    }

    return 0;
}