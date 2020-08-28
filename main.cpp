#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "main.h"
#include <chrono>

#define NUM_THREADS 8
#define NO_OF_UNIQUE_DNA 4

using namespace std;
using namespace std::chrono;

vector<char> input_vector;

void get_data(const string &filepath, vector<char> &data) {
    string line;
    ifstream myfile(filepath);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            char *c = const_cast<char *>(line.c_str());
            data.insert(data.end(), c, c + line.size());
        }
        myfile.close();
    } else cout << "Unable to open file";
}

void brute_force_search(search_params &sp, vector<int> &indices) {
    bool is_match;
    int index;

    for (int i = sp.start, j = 0; i < sp.end; i++) {
        index = i;

        if (input_vector.at(i) != sp.search_seq[j]) {
            j = 0;
            is_match = false;
        }

        if (input_vector.at(i) == sp.search_seq[j]) {
            is_match = true;
            j++;
        }

        if (is_match && j == sp.search_seq_size) {
            j = 0;
            indices.push_back(index);
        }
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

// for Boyer-Moore Bad Char Heuristic Algorithm
int hashFunction(char dna) {
    return ((int) dna/2)%4;
}

void insertItem(char dna, int index, int hashTable[NO_OF_UNIQUE_DNA]) {
    int key = hashFunction(dna);
    hashTable[key] = index;
}
 
int searchItem(char dna, int hashTable[NO_OF_UNIQUE_DNA]) {
    int key = hashFunction(dna);
    return hashTable[key];
}

void badCharHeuristic(string pattern, int hashTable[NO_OF_UNIQUE_DNA]) {
    int i{};
    for (char& c : pattern) {
        insertItem(c, i, hashTable);
        i++;
    }
}

vector<int> badCharHeuSearch(vector<char> sequence, string pattern) {
    vector<int> occurenceIndex;

    // initialize the hash table with -1
    int hashTable [NO_OF_UNIQUE_DNA] = {-1, -1, -1, -1};
    int sequenceSize = sequence.size(), patternSize = pattern.size();
    
    badCharHeuristic(pattern, hashTable);

    int offset{};

    while (offset <= (sequenceSize - patternSize)) {
        int patternIndex = patternSize - 1;

        while (patternIndex >= 0 && pattern[patternIndex] == sequence.at(patternIndex + offset)) {
            patternIndex--;
        }

        // If the pattern is present at current offset, patternIndex will be -1 after the execution of the code above
        if (patternIndex < 0) {
            occurenceIndex.push_back(offset);   // pattern found. saving index at vector

            offset += (offset + patternSize < sequenceSize) ? patternSize - searchItem(sequence[offset + patternSize], hashTable) : 1;
        }
        else {
            offset += max(1, patternIndex - searchItem(sequence[offset + patternIndex], hashTable));
        }
    }

    return occurenceIndex;
}


int main() {
    string search_seq = "TACGTCTG";
    vector<int> indices;
    search_params sp;

    auto start = high_resolution_clock::now();
    get_data("salmonella.fna", input_vector);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Fetched " << input_vector.size() << " characters in " << duration.count() << "ms" << endl << endl;

    sp.start = 0;
    sp.end = input_vector.size();
    sp.search_seq = (char *) search_seq.c_str();
    sp.search_seq_size = search_seq.size();

    //Single Threaded Brute Force
    start = high_resolution_clock::now();
    brute_force_search(sp, indices);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Single Threaded Brute Force: " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    //Multi-Threaded Brute Force
    indices.clear();
    start = high_resolution_clock::now();
    brute_force_search_multithreaded(sp, indices);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Muli-Threaded Brute Force: " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    start = high_resolution_clock::now();
    indices = badCharHeuSearch(input_vector, search_seq);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl;

    return 0;
}