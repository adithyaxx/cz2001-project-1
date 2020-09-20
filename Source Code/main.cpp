#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "brute_force.h"
#include "boyer_moore.h"
#include "kmp.h"
#include "rabin_karp.h"
#include "dual_index_search.h"

using namespace std;
using namespace std::chrono;

vector<char> input_vector;

void print(vector<int> &indices) {
    if (indices.empty()) {
        cout << "No matches found." << endl << endl;
        return;
    }

    cout << "Found matches at: ";
    for (int i = 0; i < indices.size(); i++) {
        cout << indices[i];

        if (i != (indices.size() - 1))
            cout << ",";
    }
    cout << endl << endl;
}

int main() {

    //Main UI
    //Initialization
    int option, fileopen;
    bool exit = true;
    bool gotsearchsequence = false;
    bool gotfilepath = false;
    string search_seq;
    vector<int> indices;
    search_params sp;
    string filename;
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    while (exit) {
        cout << "----------------------------------------" << endl;
        cout << "1. Specify absolute filepath" << endl;
        cout << "2. Enter search sequence" << endl;
        cout << "3. Run all algorithms" << endl;
        cout << "4. Run brute force algorithm" << endl;
        cout << "5. Run Boyer Moore algorithm" << endl;
        cout << "6. Run Knuth Morris Pratt algorithm" << endl;
        cout << "7. Run Rabin Karp algorithm" << endl;
        cout << "8. Run dual index algorithm" << endl;
        cout << "9. Exit program" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter option: ";

        cin >> option;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Wrong input! Please only enter numbers from 1 to 8" << endl;
            cout << "Enter option: ";
            cin >> option;
        }

        switch (option) {
            case 1:
                cout << "Please enter the filepath: ";
                cin >> filename;
                cout << "Loading file, please wait..." << endl;
                start = high_resolution_clock::now();

                while (!input_vector.empty())
                    input_vector.pop_back();

                fileopen = get_data(filename, input_vector);
                if (fileopen == 1) {
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Fetched " << input_vector.size() << " characters in " << duration.count() << "ms" << endl
                         << endl;
                    gotfilepath = true;
                } else {
                    cout << "Unable to open file" << endl;
                    gotfilepath = false;
                }
                break;


            case 2:
                cout << "Please enter a search sequence: ";
                cin >> search_seq;

                if (search_seq.length() > 0) {
                    sp.start = 0;
                    sp.end = input_vector.size();
                    sp.search_seq = (char *) search_seq.c_str();
                    sp.search_seq_size = search_seq.size();
                    gotsearchsequence = true;
                } else {
                    cout << "Invalid search sequence entered!" << endl;
                    gotsearchsequence = false;
                }
                break;

            case 3:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;

                    //Single Threaded Brute Force
                    indices.clear();
                    start = high_resolution_clock::now();
                    brute_force_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Single Threaded Brute Force: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Brute Force
                    indices.clear();
                    start = high_resolution_clock::now();
                    brute_force_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Muli-Threaded Brute Force: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Boyer Moore (Bad Character Heuristic)
                    indices.clear();
                    start = high_resolution_clock::now();
                    badCharHeuSearch(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Boyer Moore
                    indices.clear();
                    start = high_resolution_clock::now();
                    badCharHeuSearch_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms"
                         << endl;
                    print(indices);

                    // KMP
                    indices.clear();
                    start = high_resolution_clock::now();
                    kmp_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Knuth Morris Pratt: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded KMP
                    indices.clear();
                    start = high_resolution_clock::now();
                    kmp_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Knuth Morris Pratt: " << duration.count() << "ms" << endl;
                    print(indices);

                    indices.clear();
                    start = high_resolution_clock::now();
                    rk_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Rabin Karp: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Rabin Karp
                    indices.clear();
                    start = high_resolution_clock::now();
                    rk_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Rabin Karp: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Dual Index
                    indices.clear();
                    start = high_resolution_clock::now();
                    dual_index_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Dual Index: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Dual Index
                    indices.clear();
                    start = high_resolution_clock::now();
                    dual_index_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Dual Index: " << duration.count() << "ms" << endl;
                    print(indices);

                    break;
                }

            case 4:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;
                    //Single Threaded Brute Force
                    indices.clear();
                    start = high_resolution_clock::now();
                    brute_force_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Single Threaded Brute Force: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Brute Force
                    indices.clear();
                    start = high_resolution_clock::now();
                    brute_force_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Muli-Threaded Brute Force: " << duration.count() << "ms" << endl;
                    print(indices);

                    break;
                }

            case 5:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;
                    // Boyer Moore (Bad Character Heuristic)
                    indices.clear();
                    start = high_resolution_clock::now();
                    badCharHeuSearch(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Boyer Moore
                    indices.clear();
                    start = high_resolution_clock::now();
                    badCharHeuSearch_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Boyer Moore (Bad Character Heuristic): " << duration.count() << "ms"
                         << endl;
                    print(indices);

                    break;
                }

            case 6:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;
                    // KMP
                    indices.clear();
                    start = high_resolution_clock::now();
                    kmp_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Knuth Morris Pratt: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded KMP
                    indices.clear();
                    start = high_resolution_clock::now();
                    kmp_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Knuth Morris Pratt: " << duration.count() << "ms" << endl;
                    print(indices);

                    break;
                }

            case 7:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;
                    // RK
                    indices.clear();
                    start = high_resolution_clock::now();
                    rk_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Rabin Karp: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Rabin Karp
                    indices.clear();
                    start = high_resolution_clock::now();
                    rk_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Rabin Karp: " << duration.count() << "ms" << endl;
                    print(indices);

                    break;
                }

            case 8:
                if (gotfilepath && gotsearchsequence) {
                    cout << endl;
                    // Dual Index
                    indices.clear();
                    start = high_resolution_clock::now();
                    dual_index_search(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Dual Index: " << duration.count() << "ms" << endl;
                    print(indices);

                    // Multi-Threaded Dual Index
                    indices.clear();
                    start = high_resolution_clock::now();
                    dual_index_search_multithreaded(sp, indices);
                    end = high_resolution_clock::now();
                    duration = duration_cast<milliseconds>(end - start);
                    cout << "Multi-Threaded Dual Index: " << duration.count() << "ms" << endl;
                    print(indices);

                    break;
                }

            case 9:
                exit = false;
                break;

            default:
                if (!gotfilepath)
                    cout << "Please specify a filepath first!" << endl;
                else if (!gotsearchsequence)
                    cout << " Please specify a search sequence first!" << endl;
                else
                    cout << "Invalid option! Please try again!" << endl;
        }
    }

    return 0;
}