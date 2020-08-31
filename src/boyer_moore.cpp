//
// Created by Adithya on 31/8/20.
//
#include "../include/boyer_moore.h"

int hashFunction(char dna) {
    return ((int) dna / 2) % 4;
}

void insertItem(char dna, int index, int hashTable[NO_OF_UNIQUE_DNA]) {
    int key = hashFunction(dna);
    hashTable[key] = index;
}

int searchItem(char dna, int hashTable[NO_OF_UNIQUE_DNA]) {
    int key = hashFunction(dna);
    return hashTable[key];
}

void badCharHeuristic(const string &pattern, int hashTable[NO_OF_UNIQUE_DNA]) {
    int i = 0;
    for (char c : pattern) {
        insertItem(c, i, hashTable);
        i++;
    }
}

vector<int> badCharHeuSearch(vector<char> sequence, string pattern) {
    vector<int> occurenceIndex;

    // initialize the hash table with -1
    int hashTable[NO_OF_UNIQUE_DNA] = {-1, -1, -1, -1};
    int sequenceSize = sequence.size(), patternSize = pattern.size();

    badCharHeuristic(pattern, hashTable);

    int offset = 0;

    while (offset <= (sequenceSize - patternSize)) {
        int patternIndex = patternSize - 1;

        while (patternIndex >= 0 && pattern[patternIndex] == sequence.at(patternIndex + offset)) {
            patternIndex--;
        }

        // If the pattern is present at current offset, patternIndex will be -1 after the execution of the code above
        if (patternIndex < 0) {
            occurenceIndex.push_back(offset);   // pattern found. saving index at vector

            offset += (offset + patternSize < sequenceSize) ? patternSize -
                                                              searchItem(sequence[offset + patternSize], hashTable) : 1;
        } else {
            offset += max(1, patternIndex - searchItem(sequence[offset + patternIndex], hashTable));
        }
    }

    return occurenceIndex;
}