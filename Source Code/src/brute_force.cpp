//
// Created by Adithya on 31/8/20.
//

#include <pthread.h>
#include "utils.h"

#define NUM_THREADS 8

void brute_force_search(search_params &sp, vector<int> &indices) {
    for (int i = sp.start, j; i < sp.end - sp.search_seq_size + 1; i++) {
        for (j = 0; j < sp.search_seq_size; j++)
            if (input_vector.at(i + j) != sp.search_seq[j])
                break;

        if (j == sp.search_seq_size)
            indices.push_back(i);
    }
}

void *brute_force_search(void *ptr) {
    auto *sp = (search_params *) ptr;
    auto *indices = new vector<int>();

    brute_force_search(*sp, *indices);

    delete sp;

    return (void *) indices;
}

void brute_force_search_multithreaded(search_params &sp, vector<int> &indices) {
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

        pthread_create(&id, nullptr, &brute_force_search, (void *) sp2);
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