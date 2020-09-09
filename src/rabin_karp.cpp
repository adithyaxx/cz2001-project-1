//
// Created by Adithya on 7/9/20.
//

#include <utils.h>
#include "rabin_karp.h"
#include "pthread.h"
#include <string.h>

#define d 4
#define q 101

void rk_search(search_params &sp, vector<int> &indices) {
    int M = strlen(sp.search_seq);
    int i, j;
    int search_seq_hash = 0; // hash value for sp.search_seq
    int input_vector_hash = 0; // hash value for input_vector
    int h = 1;

    for (i = sp.start; i < sp.start + (M - 1); i++)
        h = (h * d) % q;

    for (i = 0; i < M; i++) {
        search_seq_hash = (d * search_seq_hash + sp.search_seq[i]) % q;
    }

    for (i = sp.start; i < sp.start+M; i++) {
        input_vector_hash = (d * input_vector_hash + input_vector[i]) % q;
    }

    for (i = sp.start; i <= sp.end - M; i++) {
        if (search_seq_hash == input_vector_hash) {
            for (j = 0; j < M; j++) {
                if (input_vector[i + j] != sp.search_seq[j])
                    break;
            }

            if (j == M)
                indices.push_back(i);
        }

        if (i < sp.end - M) {
            input_vector_hash = (d * (input_vector_hash - input_vector[i] * h) + input_vector[i + M]) % q;

            if (input_vector_hash < 0)
                input_vector_hash = (input_vector_hash + q);
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