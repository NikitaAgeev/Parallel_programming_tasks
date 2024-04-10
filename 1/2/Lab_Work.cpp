#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const double end_t = 1;
const double end_x = 1; 

double f(double x, double t)
{
    return expl(x)*(cosl(t) - sinl(t));
}

double start_f(double x)
{
    return expl(x)*cosl(0);
}

double left_f(double t)
{
    return expl(0)*cosl(t);
}

double u(double x, double t)
{
    return expl(x)*cosl(t);
}


int main(int argc, char* argv[])
{
    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //printf ("Communicator size= %d My rank= %d\n", commsize, my_rank );

    int I_am_left = 0;
    int I_am_right = 0;

    size_t h_n = 0;
    size_t tau_n = 0;

    if (argc == 3)
    {
        h_n = atoi(argv[1]);
        tau_n = atoi(argv[2]);
    }
    else
    {
        if(my_rank == 0){
            printf("Incorrect argv\n");
        }
        MPI_Finalize();
        return 1;
    }

    if(my_rank == 0){
        I_am_left = 1;
    }
    if(my_rank == commsize - 1){
        I_am_right = 1;
    }


    double left_data = 0;
    double right_data = 0;
    
    size_t start_i = (h_n*my_rank/commsize);
    size_t end_i = (h_n*(my_rank + 1)/commsize);

    size_t arr_size = end_i - start_i;

    double* next_arr = (double*)calloc(arr_size, sizeof(double));
    double* now_arr  = (double*)calloc(arr_size, sizeof(double));
    double* pre_arr  = (double*)calloc(arr_size, sizeof(double));

    if((next_arr == nullptr) || (now_arr == nullptr) || (pre_arr == nullptr))
    {
        printf("Your mem is bed\n");
    
        MPI_Finalize();
        return 1;
    } 


    double t = 0;
    double h = end_x/h_n;
    double tau = end_t/tau_n;

    size_t i = 0;
    
    double* ptr_buffer = nullptr;


    double max_err = 0;

    double start_time = MPI_Wtime();

    //zero layer
    for(i = start_i;  i < end_i; i += 1)
    {
        now_arr[i - start_i] = start_f(i*h);
    }
    
    if(I_am_left == 0){
        left_data = start_f((start_i - 1)*h);
    }
    if(I_am_right == 0){
        right_data = start_f((end_i)*h);
    }

    t = 0;
    //ferst layer 
    for(i = 0; i < (arr_size); i += 1)
    {
        if(I_am_left & (i == 0)){
            next_arr[i] = left_f(t + tau); 
        }
        else if (I_am_right & (i == (arr_size - 1))){
            next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i]-now_arr[i-1])/(h))*tau + now_arr[i];
        }
        else{
            if(i == 0){
                next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i+1] - left_data)/(2*h))*tau + 0.5*(now_arr[i+1] + left_data);
            }
            else if(i == (arr_size) - 1){
                next_arr[i] = (f((i + start_i)*h, t) - (right_data - now_arr[i-1])/(2*h))*tau + 0.5*(right_data + now_arr[i-1]);
            }
            else{
                next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i+1] - now_arr[i-1])/(2*h))*tau + 0.5*(now_arr[i+1] + now_arr[i-1]);
            }
        }

        #ifndef NO_ERROR_CALC
            if(abs(next_arr[i] - u((i + start_i)*h, t + tau)) > max_err)
            {
                max_err = abs(next_arr[i] - u((i + start_i)*h, t + tau));
            }
        #endif
        
    }

    MPI_Request req;

    if(I_am_right == 0)
    {
        MPI_Isend(next_arr + arr_size - 1, 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, &req);
    }
    if(I_am_left == 0)
    {
        MPI_Isend(next_arr, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, &req);
    }

    //other_layer
    for(t = tau; t < end_t; t += tau)
    {
        ptr_buffer = pre_arr;
        pre_arr = now_arr;
        now_arr = next_arr;
        next_arr = ptr_buffer;

        if(I_am_right == 0)
        {
            MPI_Irecv(&right_data, 1, MPI_DOUBLE, my_rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
            MPI_Wait (&req, MPI_STATUS_IGNORE);
        }
        if(I_am_left == 0)
        {
            MPI_Irecv(&left_data, 1, MPI_DOUBLE, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
            MPI_Wait (&req, MPI_STATUS_IGNORE);
        }
        
        for(i = 0; i < (arr_size); i += 1)
        {
            if((I_am_left) & (i == 0)){
                next_arr[i] = left_f(t + tau); 
            }
            else if ((I_am_right) & (i == (arr_size - 1))){
                next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i]-now_arr[i-1])/(h))*tau + now_arr[i];
            }
            else{
                if(i == 0){
                    next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i+1] - left_data)/(2*h))*2*tau + pre_arr[i];
                }
                else if(i == (arr_size) - 1){
                    next_arr[i] = (f((i + start_i)*h, t) - (right_data - now_arr[i-1])/(2*h))*2*tau + pre_arr[i];
                }
                else{
                    next_arr[i] = (f((i + start_i)*h, t) - (now_arr[i+1] - now_arr[i-1])/(2*h))*2*tau + pre_arr[i];
                }
            }

            #ifndef NO_ERROR_CALC
                if(abs(next_arr[i] - u((i + start_i)*h, t + tau)) > max_err)
                {
                    max_err = abs(next_arr[i] - u((i + start_i)*h, t + tau));
                }
            #endif
        }

        if(I_am_right == 0)
        {
            MPI_Isend(next_arr + arr_size - 1, 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, &req);
        }
        if(I_am_left == 0)
        {
            MPI_Isend(next_arr, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, &req);
        }


    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    #ifndef NO_ERROR_CALC
        printf("%d: %.32lf\n", my_rank, max_err);
    #endif

    if(my_rank == 0)
    {
        double end_time = MPI_Wtime();

        printf("Time = %lf mks\n", (end_time - start_time)*(1000000));//time/(size + 1));
    }
    
    free(next_arr);
    free(now_arr);
    free(pre_arr);

    MPI_Finalize();
    return 0;
}