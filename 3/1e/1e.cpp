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

    if( argc != 3){
        if(my_rank == 0)
            printf("base [I] [J]");
        MPI_Finalize();
        return 0;
    }
    else {
        I = atoi(argv[1]);
        J = atoi(argv[2]);
    }

    if((commsize % 8 != 0) & (commsize % 2 != 0))
    {
        if(my_rank == 0)
            printf("Давай, либо степень 2 либо кратно 8 потоков, а то не база\n");
        MPI_Finalize();
        return 0;
    }

    size_t tread_per_group = (commsize >= 8)? commsize/8: 1;
    size_t group_per_tread = (commsize >= 8)? 1: (commsize == 4)? 2: (commsize == 2)? 4: 8;

    size_t start_group_index = (commsize >= 8)? 1: (commsize == 4)? 2: (commsize == 2)? 4: 8;

    size_t group_i;

    size_t start_J = 

    double** group_mem_arr = calloc(,); 

    MPI_Finalize();
}