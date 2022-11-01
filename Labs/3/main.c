#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_THREADS 4

// scriere in fisier
void write_file(char *filename, char *type, double time, long sum) {
    FILE *file = fopen(filename, "w+");
    fprintf(file, "Type: %s\nTime: %lf\nSum: %ld\n", type, time, sum);
    fclose(file);
}

int main() {
    int size;
    int *numbers;

    long sum1 = 0, sum2 = 0, sum3 = 0;
    double t1, t2;
    double time1, time2, time3;
    omp_set_num_threads(NO_THREADS);

    // de paralelizat toata bucata de mai jos
    #pragma omp parallel default(none) shared(size, numbers, sum1, sum2, sum3, t1, t2, time1, time2, time3)
    {

        // fiti atenti la partea de citire din fisier
        #pragma omp master
        {
            FILE *file = fopen("input.txt", "r");
            fscanf(file, "%d", &size);
            numbers = malloc(size * sizeof(int));

            for (int i = 0; i < size; ++i) {
                fscanf(file, "%d", &numbers[i]);
            }
            fclose(file);
        }
        #pragma omp barrier // "#pragma omp master" is equivalent to "#pragma omp single nowait"

        // de paralelizat suma numerelor in 3 moduri - atomic, critical si reduction, de masurat timpii de executie
        #pragma omp single
        t1 = omp_get_wtime();
        
        #pragma omp for // "ordered" would affect performance
        for (int i = 0; i < size; ++i) {
            #pragma omp atomic
            sum1 += numbers[i];
        }
        
        #pragma omp single
        {
            t2 = omp_get_wtime();
            time1 = t2 - t1;
            t1 = omp_get_wtime();
        }
        
        #pragma omp for
        for (int i = 0; i < size; ++i) {
            #pragma omp critical
            sum2 += numbers[i];
        }
        
        #pragma omp single
        {
            t2 = omp_get_wtime();
            time2 = t2 - t1;
            t1 = omp_get_wtime();
        }
        
        #pragma omp for reduction(+:sum3)
        for (int i = 0; i < size; ++i) {
            sum3 += numbers[i];
        }
        
        #pragma omp single
        {
            t2 = omp_get_wtime();
            time3 = t2 - t1;
            free(numbers);
        }
        // "#pragma omp single" has an implied barrier at the end

        // TODO: de scris timpii de executie in 3 fisiere folosind sections (fiecare scriere intr-un section)
        #pragma omp sections
        {
            #pragma omp section
            write_file("atomic.txt", "atomic", time1, sum1);
            
            #pragma omp section
            write_file("critical.txt", "critical", time2, sum2);
            
            #pragma omp section
            write_file("reduction.txt", "reduction", time3, sum3);
        }
    }

    return 0;
}
