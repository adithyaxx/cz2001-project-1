//
// Created by Adithya on 7/9/20.
//

#include <iostream>
#include "dual_index_search.h"
#include "pthread.h"
#include <unordered_map>

#define CHAR_SET_LEN 4
#define PRIME_NUM 13

void dual_index_search(search_params &sp, vector<int> &indices) {
    int i, j;

    for (i = sp.start; i <= sp.end - sp.search_seq_size; i++) {
        if (input_vector[i] != sp.search_seq[0])
            continue;

        if (input_vector[i + sp.search_seq_size - 1] != sp.search_seq[sp.search_seq_size - 1])
            continue;

        for (j = 1; j < sp.search_seq_size - 1; j++) {
            if (input_vector[i + j] != sp.search_seq[j])
                break;
        }

        if (j == sp.search_seq_size)
            indices.push_back(i);
    }
}

void *dual_index_search(void *ptr) {
    auto *sp = (search_params *) ptr;
    auto *indices = new vector<int>();

    dual_index_search(*sp, *indices);

    delete sp;

    return (void *) indices;
}

void dual_index_search_multithreaded(search_params &sp, vector<int> &indices) {
    vector<pthread_t> threads;
    int steps = (int) input_vector.size() / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_t id;
        auto *sp2 = new search_params();


        sp2->start = i * steps;

        if (i == (NUM_THREADS - 1))
            sp2->end = input_vector.size();
        else {
            sp2->end = ((i + 1) * steps) + (sp.search_seq_size - 1);
        }

        sp2->search_seq = sp.search_seq;
        sp2->search_seq_size = sp.search_seq_size;

        pthread_create(&id, nullptr, &dual_index_search, (void *) sp2);
        threads.push_back(id);
    }

    for (pthread_t id : threads) {
        void *ret;
        pthread_join(id, &ret);
        auto *temp = (vector<int> *) ret;
        indices.insert(indices.end(), temp->begin(), temp->end());
        delete temp;
    }
}