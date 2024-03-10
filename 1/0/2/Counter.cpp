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
    double my_ans = 0;

    int i = 0;

    int N;

    if (argc == 2)
        N = atoi(argv[1]);
    else
    {
        printf("Incorrect argv\n");
        MPI_Finalize();
        return 1;
    }

    my_cnt_start = ((my_rank*N)/commsize + 1);
    my_cnt_end = (((my_rank + 1)*N)/commsize + 1);

    //printf("start: %d, end: %d \n", my_cnt_start, my_cnt_end);

    for(i = my_cnt_start; i < my_cnt_end; i++)
    {
        my_ans += 1/((double)i);
    }
    
    
    double other_buf = 0;

    MPI_Request req;

    if(my_rank != 0)
    {
        MPI_Isend(&my_ans, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req);
    }
    else
    {
        for(i = 1; i < commsize; i++)
        {
            MPI_Irecv(&other_buf, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
            MPI_Wait (&req, MPI_STATUS_IGNORE);
            my_ans += other_buf;
        }

        printf("Ans = %f\n", my_ans);
    }
    
    MPI_Finalize();
}