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
    //printf("%lu, %lu, %lu\n", I, J, N_jobs);
    

    double* a = (double*)calloc(J*I, sizeof(double));
    double* b = (double*)calloc(J*I, sizeof(double));

    size_t i;
    size_t j;

    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++)
        {
            a[i*J + j] = 10*i + j;
            b[i*J + j] = 10*i + j;
        }
    }

    #ifdef TEST
    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++)
        {
            printf("10*i + j: %d, %lf\n", 10*i + j, a[i*J + j]);
            fflush(stdout);
        }
    }
    #endif

    fflush(stdout);

    double start_time = omp_get_wtime();
    
    #pragma omp parallel
    {
        int tread = omp_get_thread_num();
        size_t start_J = (J*tread)/N_jobs;
        size_t end_J = (J*(tread + 1))/N_jobs;

        if(end_J == J)
            end_J = end_J - 2;

        size_t i;
        size_t j;

        for(i = 0; i < I-4; i++)
        {
            for(j = start_J; j < end_J; j++)
            {
                b[i*J + j] = sin(0.1*a[(i+4)*J + j + 2]);
            }
        }
    }

    double end_time = omp_get_wtime();

    fflush(stdout);

    #ifdef TEST_OUT
    FILE* file = fopen("out.txt", "w");
    for(j = 0; j < J; j++){
        fprintf(file, "%d\t", j);
    }
    fprintf(file, "\n");
    for(i = 0; i < I; i++)
    {
        for(j = 0; j < J; j++){
            fprintf(file, "%lf\t", b[i*J + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    #endif
    
    #ifdef TEST
        for(i = 0; i < I; i++)
        {
            for(j = 0; j < J; j++)
            {
                printf("%lf\n", a[i*J + j]);
            }
        }
    #endif

    fflush(stdout);

    free(a);
    free(b);
    
    printf("Time: %f s\n", ((float)end_time - (float)start_time));//time/(size + 1));

}