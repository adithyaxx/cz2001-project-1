//
// Created by Adithya on 31/8/20.
//

#include "../include/kmp.h"
#include <pthread.h>

void create_kmp_table(const char *search_seq, int *table) {
    int len = 0;
    int i = 1;

    table[0] = 0;

    while (i < (int) strlen(search_seq)) {
        if (search_seq[i] == search_seq[len]) {
            len++;
            table[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = table[len - 1];
            } else {
                table[i] = 0;
                i++;
            }
        }
    }
}

void kmp_search(search_params &sp, vector<int> &indices) {
    int i = sp.start;
    int j = 0;
    int table[sp.search_seq_size];

    create_kmp_table(sp.search_seq, table);

    while (i < sp.end) {
        if (sp.search_seq[j] == input_vector.at(i)) {
            j++;
            i++;
        }

        if (j == sp.search_seq_size) {
            indices.push_back(i - j);
            j = table[j - 1];
        } else if (i < sp.end && sp.search_seq[j] != input_vector.at(i)) {
            if (j != 0)
                j = table[j - 1];
            else
                i++;
        }
    }
}

void *kmp_search(void *ptr) {
    auto *sp = (search_params *) ptr;
    auto *indices = new vector<int>();

    kmp_search(*sp, *indices);

    delete sp;

    return (void *) indices;
}

void kmp_search_multithreaded(search_params &sp, vector<int> &indices) {
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

        pthread_create(&id, nullptr, &kmp_search, (void *) sp2);
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