#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    unsigned int size;
    unsigned int *v;
    unsigned long long sum_serial = 0;
    unsigned long long sum_parallel = 0;
    
    scanf("%u", &size);
    v = malloc(size * sizeof(unsigned int));
    if (v == NULL)
    {
        exit(12);
    }
    for (unsigned int i = 0; i < size; ++i)
    {
        v[i] = i;
        sum_serial += v[i];
    }

    #pragma omp parallel default(none) shared(size, v, sum_parallel)
    {
        #pragma omp for
        for (unsigned int i = 0; i < size; ++i)
        {
            #pragma omp critical
            sum_parallel += v[i];
        }
    }

    printf("Serial sum = %llu\n", sum_serial);
    printf("Parallel sum = %llu\n", sum_parallel);
    printf(sum_serial == sum_parallel ? "Correct!\n" : "Wrong!\n"); 
    
    free(v);
    return 0;
}
