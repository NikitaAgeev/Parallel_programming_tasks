#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    
    size_t I, J, N_jobs;

    
    if( argc != 4){
        printf("base [I] [J] [N_jobs]\n");
        return 0;
    }
    else {
        I = atoi(argv[1]);
        J = atoi(argv[2]);
        N_jobs = atoi(argv[3]);
    }

    int MAX_THREAD = omp_get_max_threads();
    if(N_jobs > MAX_THREAD){
        printf("Многа берёшь, только %u\n", MAX_THREAD);
        N_jobs = MAX_THREAD;
    }
    
    #ifdef TEST
        printf("MAX_TREAD: %d \n", MAX_THREAD);
    #endif
    


    omp_set_num_threads(N_jobs);
    printf("%lu, %lu, %lu\n", I, J, N_jobs);
    

    double* a = (double*)calloc(J*I, sizeof(double));

    size_t i;
    size_t j;

    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++)
        {
            a[i*J + j] = 10*i + j;
        }
    }

    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++)
        {
            printf("10*i + j: %d, %d\n", 10*i + j, a[i*J + j]);
            fflush(stdout);
        }
    }

    fflush(stdout);

    time_t start_time = clock();

    #pragma omp parallel
    {
        int tread = omp_get_thread_num();
        size_t start_J = (J*tread)/N_jobs;
        size_t end_J = (J*(tread + 1))/N_jobs;

        size_t i;
        size_t j;

        for(i = 0; i < I; i++)
        {
            for(j = start_J; j < end_J; j++)
            {
                a[i*J + j] = sin(a[i*J + j]);
            }
        }
    }

    time_t end_time = clock();

    fflush(stdout);

    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++)
        {
            printf("%d\n", a[i*J + j]);
        }
    }

    fflush(stdout);

    free(a);
    
    printf("Time: %f mks\n", ((float)end_time - (float)start_time));//time/(size + 1));

}