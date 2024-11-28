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
    
    int threads = (MAX_THREAD > N)? N: MAX_THREAD;
    omp_set_num_threads(threads);
    printf("Treads_num: %d\n", threads);

    double ans;


    #pragma omp parallel reduction(+:ans)
    {
        int i = 0;
        int thread = omp_get_thread_num();
        int w_size = N/threads;
        for(i = (w_size*thread) + 1; i < (w_size*(thread + 1)) + 1; i += 1)
        {
            ans += 1/((double)i);
            #ifdef TEST 
                printf("%d, %d, %lf\n", thread, i, ans);
            #endif
        }
    }

    printf("ans: %lf\n", ans);
}