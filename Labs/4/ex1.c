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
    
    if (rank == 0)
    {
        int size = numProc * CHUNK;
        int v[size];
        srand(time(NULL));
        for (int i = 0; i < size; ++i)
        {
            v[i] = rand() % 100;
        }
        for (int i = 1; i < numProc; ++i)
        {
            MPI_Send(v + i * CHUNK, CHUNK, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        printf("Master sent\n");
        int sum[numProc];
        sum[0] = 0;
        for (int i = 0; i < CHUNK; ++i)
        {
            sum[0] += v[i];
        }
        for (int i = 1; i < numProc; ++i)
        {
            MPI_Recv(&sum[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("Master received\n");
        int res = sum[0];
        for (int i = 1; i < numProc; ++i)
        {
            res += sum[i];
        }
        printf("Sum is %d\n", res);
    }
    else
    {
        int v[CHUNK];
        MPI_Recv(v, CHUNK, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received\n", rank);
        int sum = 0;
        for (int i = 0; i < CHUNK; ++i)
        {
            sum += v[i];
        }
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Rank %d sent\n", rank);
    }
    
    MPI_Finalize();
    return 0;
}
