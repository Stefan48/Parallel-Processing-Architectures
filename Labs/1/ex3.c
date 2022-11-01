#include <stdio.h>
#include <omp.h>

int main(void)
{
	omp_set_num_threads(4);
	omp_set_nested(1);
    #pragma omp parallel
    {
    	printf("Hello (outer) from thread %d\n", omp_get_thread_num());
    	#pragma omp parallel
		{
			printf("Hello (inner) from thread %d\n", omp_get_thread_num());
		}
    }
	
    return 0;
}
