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

    size_t calk_per_tread = (commsize >= 8)? 1 : (commsize == 4)? 2 : (commsize == 2)? 4 : 8;
    size_t calk_per_group = (commsize >= 8)? commsize/8 : 1;

    size_t start_J = (J*my_rank)/calk_per_group;
    size_t end_J = (J*(my_rank + 1))/calk_per_group;
    size_t w_J = end_J - start_J;
    
    double** groop_arr = (double**)calloc(calk_per_tread, sizeof(double*));
    
    size_t groop = 0;
    for(groop = 0; groop < calk_per_tread; groop++){
        groop_arr[groop] = (double*)calloc(w_J*I, sizeof(double));
    }

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
    
    if(calk_per_group == 1){
        for(i = 0; i < I; i++)
        {
            for(j = start_J; j < end_J; j++)
            {
                a[i*w_J + j-start_J] = sin(a[i*w_J + j-start_J]);
            }
        }    
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    free(a);

    if(my_rank == 0)
        printf("Time: %lf mks\n", (end_time - start_time)*(1000000));//time/(size + 1));


    MPI_Finalize();
}