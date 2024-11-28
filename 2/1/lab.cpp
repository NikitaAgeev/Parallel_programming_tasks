#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

double eps;
double real_ans = 2.72611738439407; 
int N;

double f (double x){
    return sin(1/x);
}

double dd_f (double x){
    return -sin(1/x)/(x*x*x*x) + 2*cos(1/x)/(x*x*x);
}

pthread_mutex_t x_l;
//#define NO_PAR
void* func(void* argv){
    
    #ifdef NO_PAR
    pthread_mutex_lock(&x_l);
    #endif

    double* res = (double*)calloc(1, sizeof(double));
    
    double start = ((double*)argv)[0];
    double stop = ((double*)argv)[1];

    double max_dd_f = 0;
    if((start < 1) & (stop > 1))
        max_dd_f = 1;

    double max = (abs(dd_f(start)) > abs(dd_f(stop)))? abs(dd_f(start)): abs(dd_f(stop)); 
    max_dd_f = (max > max_dd_f)? max: max_dd_f;

    double h = (12*eps)/(N*max_dd_f*(stop - start));
    h = std::sqrt(h);
    size_t N = (stop - start)/h + 1;
    h = (stop - start)/N;

    for(int i = 1; i < N; i++){
        *res += f(i*h + start);
    }

    *res += (f(start) + f(stop))/2;

    *res *= h;

    #ifdef NO_PAR
    pthread_mutex_unlock(&x_l);
    #endif

    printf("%lf:%lf - %lf == %lf (max: %lf) (eps: %lu)\n", start, stop, h, *res, max_dd_f, N);

    pthread_exit(res);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Wrong argv\n [End N] [tread num] [eps]");
        return 1;
    }

    double start_time = clock();
    pthread_mutex_init(&x_l, NULL);

    double start = 0.01;
    double stop = 10;
    
    N = atoi(argv[1]);
    eps = atof(argv[2]);
    pthread_t arr [N];
    double arg_arr [N][2];

    for(int i = 0; i < N; i++){
        arg_arr[i][0] = (((stop - start)*i)/N) + start;
        arg_arr[i][1] = (((stop - start)*(i+1))/N) + start;
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

    double end_time = clock();

    printf("ans %lf\n", ans);
    printf("err: %lf (%lf%%)\n", abs(real_ans -ans), 100*abs(real_ans - ans)/real_ans);
    printf("time: %lf ms\n", end_time - start_time);
    return 0;
}