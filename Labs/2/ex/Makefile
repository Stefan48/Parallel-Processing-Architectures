build:
	gcc atan.c -o atan -fopenmp
	g++ count_letters.cpp -o count_letters -fopenmp
	g++ count_letters_parallel.cpp -o count_letters_parallel -fopenmp
	gcc schedule.c -o schedule -fopenmp
clean:
	rm atan schedule count_letters count_letters_parallel
