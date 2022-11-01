#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define CHUNK 128

int main (int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    
    int numProc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int *arr, *part, *sums;
    part = malloc(CHUNK * sizeof(int));
    
    if (rank == 0)
    {
        int size = numProc * CHUNK;
        arr = malloc(size * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < size; ++i)
        {
            arr[i] = rand() % 100;
        }
        int coefficient = rand() % 100;
        MPI_Bcast(&coefficient, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(arr, CHUNK, MPI_INT, part, CHUNK, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Master sent\n");
        sums = malloc(numProc * sizeof(int));
        int sum = 0;
        for (int i = 0; i < CHUNK; ++i)
        {
            sum += part[i];
        }
        sum *= coefficient;
        printf("Rank %d: Sum is %d\n", rank, sum);
        MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Master received\n");
        int res = 0;
        for (int i = 0; i < numProc; ++i)
        {
            res += sums[i];
        }
        printf("Sum is %d\n", res);
        free(arr);
        free(sums);
    }
    else
    {
        int coefficient;
        MPI_Bcast(&coefficient, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(arr, CHUNK, MPI_INT, part, CHUNK, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Rank %d received\n", rank);
        int sum = 0;
        for (int i = 0; i < CHUNK; ++i)
        {
            sum += part[i];
        }
        sum *= coefficient;
        printf("Rank %d: Sum is %d\n", rank, sum);
        MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Rank %d sent\n", rank);
    }
    
    free(part);
    MPI_Finalize();
    return 0;
}
