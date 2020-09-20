 //
// Created by Adithya on 31/8/20.
//

#ifndef CZ2001_PROJECT_1_UTILS_H
#define CZ2001_PROJECT_1_UTILS_H

#include <vector>
#include <string>

#define NUM_THREADS 8

using namespace std;

extern vector<char> input_vector;

typedef struct search_params_t {
    int start = 0;
    int end = 0;
    const char *search_seq = nullptr;
    int search_seq_size = 0;
} search_params;

int get_data(const string &filepath, vector<char> &data);

#endif //CZ2001_PROJECT_1_UTILS_H
