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

    int my_cnt_start = 0;
    int my_cnt_end   = 0;
    int my_number    = 10;

    MPI_Request req;

    if(my_rank != 0)
    {
        MPI_Irecv(&my_number, 1, MPI_INTEGER, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
        MPI_Wait (&req, MPI_STATUS_IGNORE);

        my_number++;
    }

    printf("Rank: %d, my_number: %d\n", my_rank, my_number);

    MPI_Isend(&my_number, 1, MPI_INTEGER, ((my_rank + 1) < commsize)? my_rank + 1: 0, 0, MPI_COMM_WORLD, &req);

    if(my_rank == 0)
    {
        MPI_Irecv(&my_number, 1, MPI_INTEGER, commsize - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
        MPI_Wait (&req, MPI_STATUS_IGNORE);

        my_number++;
        printf("Rank: %d, my_number: %d\n", my_rank, my_number);
    }

    MPI_Finalize();
}