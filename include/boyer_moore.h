//
// Created by Adithya on 31/8/20.
//

#ifndef CZ2001_PROJECT_1_BOYER_MOORE_H
#define CZ2001_PROJECT_1_BOYER_MOORE_H

#include <vector>
#include <string>
#include "utils.h"

#define TABLE_SIZE 5

using namespace std;

void badCharHeuSearch(search_params &sp, vector<int> &indices);

void badCharHeuSearch_multithreaded(search_params &sp, vector<int> &indices);
#endif //CZ2001_PROJECT_1_BOYER_MOORE_H
