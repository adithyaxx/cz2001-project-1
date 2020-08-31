//
// Created by Adithya on 31/8/20.
//

#ifndef CZ2001_PROJECT_1_KMP_H
#define CZ2001_PROJECT_1_KMP_H

#include "utils.h"

void kmp_search(search_params &sp, vector<int> &indices);

void kmp_search_multithreaded(search_params &sp, vector<int> &indices);

#endif //CZ2001_PROJECT_1_KMP_H
