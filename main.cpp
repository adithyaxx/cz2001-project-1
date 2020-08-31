#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "brute_force.h"
#include "boyer_moore.h"
#include "kmp.h"

using namespace std;
using namespace std::chrono;

vector<char> input_vector;

int main() {
    string search_seq = "ACTGACA";
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
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    // Boyer Moore (Bad Character Heuristic)
    start = high_resolution_clock::now();
    indices = badCharHeuSearch(input_vector, search_seq);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    // KMP
    indices.clear();
    start = high_resolution_clock::now();
    kmp_search(sp, indices);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Knuth Morris Pratt: " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    // KMP Multi-Threaded
    indices.clear();
    start = high_resolution_clock::now();
    kmp_search_multithreaded(sp, indices);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Multi-Threaded Knuth Morris Pratt: " << duration.count() << "ms" << endl;
    cout << "Found " << indices.size() << " matches!" << endl << endl;

    return 0;
}