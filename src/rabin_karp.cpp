//
// Created by Adithya on 7/9/20.
//

#include <iostream>
#include "rabin_karp.h"
#include "pthread.h"
#include <unordered_map>

#define CHAR_SET_LEN 4
#define PRIME_NUM 13

void rk_search(search_params &sp, vector<int> &indices) {
    int i, j;
    int search_seq_hash = 0; // hash value for sp.search_seq
    int input_vector_hash = 0; // hash value for input_vector
    int h = 1;

    for (i = sp.start; i < sp.start + (sp.search_seq_size - 1); i++) {
        h = (h * CHAR_SET_LEN) % PRIME_NUM;
    }

    for (i = 0; i < sp.search_seq_size; i++) {
        search_seq_hash = (search_seq_hash * CHAR_SET_LEN + sp.search_seq[i]) % PRIME_NUM;
    }

    for (i = sp.start; i < sp.start + sp.search_seq_size; i++) {
        input_vector_hash = (input_vector_hash * CHAR_SET_LEN + input_vector[i]) % PRIME_NUM;
    }

    for (i = sp.start; i <= sp.end - sp.search_seq_size; i++) {
        if (search_seq_hash == input_vector_hash) {
            for (j = 0; j < sp.search_seq_size; j++) {
                if (input_vector[i + j] != sp.search_seq[j])
                    break;
            }

            if (j == sp.search_seq_size)
                indices.push_back(i);
        }

        if (i < sp.end - sp.search_seq_size) {
            input_vector_hash =
                    ((input_vector_hash - input_vector[i] * h) * CHAR_SET_LEN + input_vector[i + sp.search_seq_size]) %
                    PRIME_NUM;

            if (input_vector_hash < 0)
                input_vector_hash += PRIME_NUM;
        }
    }
}

void *rk_search(void *ptr) {
    auto *sp = (search_params *) ptr;
    auto *indices = new vector<int>();

    rk_search(*sp, *indices);

    delete sp;

    return (void *) indices;
}

void rk_search_multithreaded(search_params &sp, vector<int> &indices) {
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

        pthread_create(&id, nullptr, &rk_search, (void *) sp2);
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