#include <stdio.h>
#include <omp.h>
#define N 10000000
int a[N];
int main(void) {

    for (int i = 0; i < N; i++)
    {
        a[i] = 1;
	}
    int sum = 0;
    double t1, t2;
    t1 = omp_get_wtime();
    #pragma omp parallel default(none) shared(sum, a)
    {
		#pragma omp for
		for (int i = 0; i < N; ++i)
		{
			#pragma omp critical
			sum += a[i];
		}
	}
    t2 = omp_get_wtime();
    printf("Sum=%d, duration=%g\n", sum, t2 - t1);
    return 0;
}

/* Check out
http://jakascorner.com/blog/2016/06/omp-data-sharing-attributes.html
for some good practice tips when it comes to data sharing attributes
*/
