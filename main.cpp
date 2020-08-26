#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "main.h"

#define NUM_THREADS 8

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
        if (input_vector.at(i) == sp.search_seq[j]) {
            is_match = true;
            j++;
        } else {
            j = 0;
            is_match = false;
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
        sp2->end = (i + 1) * steps;
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

int main() {
    string search_seq = "TAGTTG";
    vector<int> indices;
    search_params sp;

    auto start = high_resolution_clock::now();
    get_data("../genome.fna", input_vector);
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
    cout << "Found " << indices.size() << " matches!" << endl;

    return 0;
}