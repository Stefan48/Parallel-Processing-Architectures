#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNKSIZE 10
#define N 100000

int main(int argc, char *argv[]) 
{
    int nthreads, tid, i, chunk;
    float a[N], b[N], c[N];
        
    /* Some initializations */
    for (i = 0; i < N; i++) {
        a[i] = b[i] = i * 1.0;
    }
    chunk = CHUNKSIZE;

    double t1, t2;
    t1 = omp_get_wtime();
    #pragma omp parallel shared(a, b, c, nthreads, chunk) private(i, tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        #pragma omp for schedule(dynamic, chunk)
        for (i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }

    }
    t2 = omp_get_wtime();
    
    printf("Execution time %g\n",t2-t1);
    
    return 0;
}

/* Results:
 - static chunk=1 => time=0.001
 - static chunk=10 => time=0.001
 - static chunk=100 => time=0.001
 - dynamic chunk=1 => time=0.014
 - dynamic chunk=10 => time=0.002
 - dynamic chunk=100 => time=0.001
 - guided chunk=1 => time=0.0009
 - guided chunk=10 => time=0.0008
 - guided chunk=100 => time=0.0009
 - auto => time=0.0008
*/
