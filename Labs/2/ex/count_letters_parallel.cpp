#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#define NUM_THREADS 4

using namespace std;

int main() {
    map<char, int> letters;
    vector<string> words;

    for (int i = 32; i < 128; i++) {
        letters[(char)i] = 0;
    }
    
    omp_set_num_threads(NUM_THREADS);
    
    #pragma omp parallel default(none) shared(words, letters)
	{
		#pragma omp for schedule(auto)
		for (int i = 1; i <= 100; i++) {
		    string filename = "files/f" + to_string(i);
		    char content[10000];
		    FILE *file;

		    file = fopen(filename.c_str(),"r");
		    while (fgets(content, sizeof(content), file) != NULL) {
		    	#pragma omp critical
		    	words.push_back(string(content));
		    }
		    // string temp(content);
		    // words.push_back(string(content));
		    fclose(file);
		}
		
		#pragma omp barrier

		#pragma omp for schedule(auto)
		for (int i = 0; i < words.size(); i++) {
		    for (int j = 0; j < words[i].size(); j++) {
		    	#pragma omp critical
		        letters[words[i][j]]++;
		    }
		}
	}

    for (int i = 32; i < 128; i++) {
        cout << (char)i << ": " << letters[char(i)] << endl;
    }
}
