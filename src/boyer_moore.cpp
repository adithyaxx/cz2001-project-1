//
// Created by Adithya on 31/8/20.
//
#include "boyer_moore.h"
#include <pthread.h>
#include <iostream>

int hashFunction(char dna) {
    return (int) dna % TABLE_SIZE;
}

void insertItem(char dna, int index, int hashTable[TABLE_SIZE]) {
    int key = hashFunction(dna);
    hashTable[key] = index;
}

int searchItem(char dna, int hashTable[TABLE_SIZE]) {
    int key = hashFunction(dna);
    return hashTable[key];
}

// pre-processing function
void badCharHeuristic(const string &pattern, int hashTable[TABLE_SIZE]) {
    int i = 0;
    for (char c : pattern) {
        insertItem(c, i, hashTable);
        i++;
    }
}

void badCharHeuSearch(search_params &sp, vector<int> &indices) {
    // initialize the hash table with -1
    int hashTable[TABLE_SIZE] = {-1, -1, -1, -1, -1};
    int sequenceSize = sp.end, patternSize = sp.search_seq_size;

    badCharHeuristic(sp.search_seq, hashTable);

    int offset = sp.start;

    while (offset <= (sequenceSize - patternSize)) {
        int patternIndex = patternSize - 1;

        while (patternIndex >= 0 && sp.search_seq[patternIndex] == input_vector.at(patternIndex + offset)) {
            patternIndex--;
        }

        // If the pattern is present at current offset, patternIndex will be -1 after the execution of the code above
        if (patternIndex < 0) {
            indices.push_back(offset);   // pattern found. saving index at vector

            offset += (offset + patternSize < sequenceSize) ? patternSize - searchItem(input_vector.at(offset + patternSize), hashTable) : 1;
        } else {
            offset += max(1, patternIndex - searchItem(input_vector.at(offset + patternIndex), hashTable));
        }
    }
}

void *booyer_moore_search(void *ptr) {
    auto *sp = (search_params *) ptr;
    auto *indices = new vector<int>();

    badCharHeuSearch(*sp, *indices);

    delete sp;

    return (void *) indices;
}

void badCharHeuSearch_multithreaded(search_params &sp, vector<int> &indices) {
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

        pthread_create(&id, nullptr, &booyer_moore_search, (void *) sp2);
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