#include <stdio.h>
#include <omp.h>
#define N 10000000
int a[N];
int main(void) {

    int i;
    for (i = 0; i < N; i++)
        a[i] = 1;

    int sum = 0;
    double t1, t2;
    t1 = omp_get_wtime();
    int tid, numt;
    #pragma omp parallel default(shared) private(i, tid)
    {
		tid = omp_get_thread_num();
		numt = omp_get_num_threads();
		int range = N / numt;
		int from = range * tid;
		int to = from + range - 1;
		if (to >= N)
		{
			to = N - 1;
		}
		for (i = from; i <= to; ++i)
		{
			#pragma omp critical
			sum += a[i];
		}
	}
    t2 = omp_get_wtime();
    
    printf("Sum=%d, duration=%g\n", sum, t2 - t1);
    return 0;
}
