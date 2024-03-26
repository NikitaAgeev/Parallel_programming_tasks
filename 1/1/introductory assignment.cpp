#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //printf ("Communicator size= %d My rank= %d\n", commsize, my_rank );

    double my_data = 0.01;

    MPI_Request req;

    int other_rank = (my_rank == 0)? 1: 0; 

    size_t i = 0;
    size_t size = 100000;

    double start_time = MPI_Wtime();

    
    if(my_rank == 1 || my_rank == 0)
    {
        if(my_rank == 0)
        {
            MPI_Isend(&my_data, 1, MPI_DOUBLE, other_rank, 0, MPI_COMM_WORLD, &req);
        }

        for(i = 0; i < size; i++)
        {
            MPI_Irecv(&my_data, 1, MPI_DOUBLE, other_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
            MPI_Wait (&req, MPI_STATUS_IGNORE);

            //printf("%d, %lf\n", i, my_data);

            my_data += 0.01;
            
            MPI_Isend(&my_data, 1, MPI_DOUBLE, other_rank, 0, MPI_COMM_WORLD, &req);
        }

        if(my_rank != 0)
        {
            MPI_Irecv(&my_data, 1, MPI_DOUBLE, other_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
            MPI_Wait (&req, MPI_STATUS_IGNORE);
        }
    }

    if(my_rank != 0)
    {
        double end_time = MPI_Wtime();

        printf("Ans = %lf mks\n", (end_time - start_time)*(1000000/(size + 1)));//time/(size + 1));
    }

    MPI_Finalize();
}