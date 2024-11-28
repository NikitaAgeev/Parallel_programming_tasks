#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char* argv[])
{
    int thread;
    int threads = 10;
    omp_set_num_threads(10);
    #pragma omp parallel private(thread)
    {
        int i = 0;
        thread = omp_get_thread_num();
        printf("Hello world %d/%d\n", thread, threads);
    }
}