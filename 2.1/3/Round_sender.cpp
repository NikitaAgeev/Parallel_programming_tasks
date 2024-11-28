#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("ABOBA\n");
        return 0;
    }
    int N = atoi(argv[1]);

    int MAX_THREAD = omp_get_max_threads();
    #ifdef TEST
        printf("MAX_TREAD: %d \n", MAX_THREAD);
    #endif
    
    int threads = N;
    omp_set_num_threads(threads);
    printf("Treads_num: %d\n", threads);

    int I = 20;
    int order = 0;
    omp_lock_t I_locker;
    omp_init_lock(&I_locker);

    #pragma omp parallel
    {
        int tread = omp_get_thread_num();
        int my_order = 0;
        while(my_order == 0)
        {
            if(omp_test_lock(&I_locker))
            {
                if(tread != order)
                {
                    //printf("Hello, my name is %d. I'm miss.\n", tread);
                    omp_unset_lock(&I_locker);
                }
                else
                {
                    //printf("Hello, my name is %d. I'm start worck.\n", tread);
                    my_order = 1;
                    order++;
                }
            }
        }
        printf("Hello, my name is %d. I'm take %d.\n", tread, I);
        I = I + 1;
        omp_unset_lock(&I_locker); 
    }

    printf("Hello, my name is master. I'm take %d.\n", I);
}