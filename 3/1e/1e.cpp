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

    if((commsize <= 8) & (commsize % 2 != 0) & (commsize != 1))
    {
        if(my_rank == 0)
            printf("Давай, степень 2 и не больше 8 потоков, а то не база\n");
        MPI_Finalize();
        return 0;
    }

    size_t tread_per_group = (commsize >= 8)? commsize/8: 1;
    size_t group_per_tread = (commsize >= 8)? 1: (commsize == 4)? 2: (commsize == 2)? 4: 8;
    size_t n_gr_off_gr     = (commsize >= 8)? 8: commsize;

    size_t start_group_index = (my_rank % n_gr_off_gr) * group_per_tread;
    size_t start_gr_of_gr_index = my_rank / 8;

    int primer = (start_gr_of_gr_index == 0);

    size_t group_i;

    size_t start_J = (J*start_gr_of_gr_index)/tread_per_group;
    size_t end_J =   (J*(start_gr_of_gr_index + 1))/tread_per_group;
    size_t w_J = end_J - start_J;
    size_t real_w_J = w_J/8;
    real_w_J = (primer)? real_w_J + 1: real_w_J;

    double** group_mem_arr = (double**)calloc(group_per_tread, sizeof(double*)); 
    if(group_mem_arr == NULL)
    {
        printf("с памятью проблемы\n");
        MPI_Finalize();
        return 0;
    }
    for (group_i = 0; group_i < group_per_tread; group_i++)
    {
        group_mem_arr[group_i] = (double*)calloc(real_w_J*I, sizeof(double));
        if(group_mem_arr[group_i] == NULL)
        {
            printf("с памятью проблемы\n");
            MPI_Finalize();
            return 0;
        }
    }

    #ifdef TEST
        printf("my_rank: %u, s_J: %lu, e_J: %lu, tpg: %lu, gpt: %lu, sgi: %lu, rwj: %lu\n", my_rank, start_J, end_J, tread_per_group, group_per_tread, start_group_index, real_w_J);
    #endif

    size_t i;
    size_t j;
    size_t j_real;

    for(i = 0; i < I; i++)
    {
        for(j = start_J; j < end_J; j=j+8)
        {
            for(group_i = 0; group_i < group_per_tread; group_i++){
                j_real = j + start_group_index + group_i;
                group_mem_arr[group_i][i*real_w_J + j/8] = 10*i + j_real;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    double* gr_at_pr;

    for(group_i = 0; group_i < group_per_tread; group_i++)
    {
        gr_at_pr = group_mem_arr[group_i];
        #ifdef TEST
        //printf("r: %u, gi: %lu\n",my_rank ,group_i);
        #endif
        for(i = 1; i < I; i++)
        {
            for(j = start_J + 8 + start_group_index + group_i, j_real = 1; j < end_J; j=j+8, j_real++){
                gr_at_pr[i*real_w_J + j_real] = sin(5*gr_at_pr[(i-1)*real_w_J + j_real - 1]);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if(my_rank == 0)
        printf("Time: %lf mks\n", (end_time - start_time)*(1000000));//time/(size + 1));

    #ifdef TEST_OUT
    for (group_i = 0; group_i < group_per_tread; group_i++)
    {
        char name_bufer[50];
        sprintf(name_bufer,"%d.%d.txt", my_rank, group_i);
        FILE* file = fopen(name_bufer, "w");
        for(j = start_J + start_group_index + group_i; j < end_J; j=j+8){
            fprintf(file, "%d\t", j);
        }
        fprintf(file, "\n");
        for(i = 0; i < I; i++)
        {
            for(j = start_J + start_group_index + group_i, j_real = 0; j < end_J; j=j+8, j_real++){
                fprintf(file, "%lf\t", group_mem_arr[group_i][i*real_w_J + j_real]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
    #endif

    for (group_i = 0; group_i < group_per_tread; group_i++)
    {
        free(group_mem_arr[group_i]);
    }
    free(group_mem_arr);

    MPI_Finalize();
}