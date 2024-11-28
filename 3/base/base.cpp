#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //printf ("Communicator size= %d My rank= %d\n", commsize, my_rank );

    size_t I;
    size_t J;

    if( argc != 3)
        printf("base [I] [J]");
    else {
        I = atoi(argv[1]);
        J = atoi(argv[2]);
    }

    size_t start_J = (J*my_rank)/commsize;
    size_t end_J = (J*(my_rank + 1))/commsize;
    size_t w_J = end_J - start_J;
    
    double* a = (double*)calloc((end_J - start_J)*(I), sizeof(double));

    size_t i;
    size_t j;

    for(i = 0; i < I; i++)
    {
        for(j = start_J; j < end_J; j++)
        {
            a[i*w_J + j-start_J] = 10*i + j;
        }
    }

    double start_time = MPI_Wtime();
    
    for(i = 0; i < I; i++)
    {
        for(j = start_J; j < end_J; j++)
        {
            a[i*w_J + j-start_J] = sin(a[i*w_J + j-start_J]);
        }
    }

    double end_time = MPI_Wtime();
    free(a);

    printf("Ans = %lf mks\n", (end_time - start_time)*(1000000));//time/(size + 1));


    MPI_Finalize();
}