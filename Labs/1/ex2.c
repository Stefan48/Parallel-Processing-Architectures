#include <stdio.h>
#include <omp.h>
#define N 10000000
int a[N];
int main(void) {

    int i;
    for (i=0;i<N;i++)
        a[i] = 1;

    int sum = 0;
    double t1, t2;
    t1 = omp_get_wtime();
    for (i=0;i<N;i++)
        sum += a[i];
    t2 = omp_get_wtime();
    
    printf("Sum=%d, duration=%g\n",sum,t2-t1);
    return 0;
}
