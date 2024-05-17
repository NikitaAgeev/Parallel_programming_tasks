#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

void* func(void* argv){
    double* res = (double*)calloc(1, sizeof(double));
    for(int i = ((int*)argv)[0]; i < ((int*)argv)[1]; i++){
        *res += 1/((double)i);
    }

    pthread_exit(res);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wrong argv\n [End N] [tread num]");
        return 1;
    }
    
    int N = atoi(argv[1]);
    int end_N = atoi(argv[2]);
    pthread_t arr [N];
    int arg_arr [N][2];

    for(int i = 0; i < N; i++){
        arg_arr[i][0] = (end_N*i/N) + 1;
        arg_arr[i][1] = (end_N*(i+1)/N) + 1;
        //printf("%d %d %d\n", i, arg_arr[i][0], arg_arr[i][1]);
        pthread_create(&arr[i], NULL, func, (void*)&arg_arr[i]);
    }

    double ans = 0;
    double* param = NULL;
    for(int i = 0; i < N; i++){
        pthread_join(arr[i], (void**)&param);
        //printf("ans %d: %lf\n",i ,*param);
        ans += *param;
        free(param);
    }

    printf("ans %lf\n", ans);

    return 0;
}