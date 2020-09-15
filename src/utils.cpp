//
// Created by Adithya on 31/8/20.
//

#include <fstream>
#include <iostream>
#include "../include/utils.h"

int get_data(const string &filepath, vector<char> &data) {
    string line;
    ifstream myfile(filepath);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            char *c = const_cast<char *>(line.c_str());
            data.insert(data.end(), c, c + line.size());
        }
        myfile.close();
        return 1;
    } else
        return 0;

}