#ifndef GRIDTABLE_H
#define GRIDTABLE_H
#include <bits/stdc++.h>
using namespace std;

struct GridTable {
    vector<int> rows, cols;
    vector<vector<int> > data;

    GridTable(const vector<pair<char, int> > &a_compressed, const int n, const vector<pair<char, int> > &b_compressed,
              const int m) {
        rows = vector(n + 1, -1);
        cols = vector(m + 1, -1);
        rows[0] = 0;
        cols[0] = 1;
        data.push_back(vector<int>(m + 1, 0));
        data.push_back(vector<int>(n + 1, 0));

        int place = 0;
        for (int i = 0; i < a_compressed.size(); i++) {
            place += a_compressed[i].second;
            rows[place] = data.size();
            data.push_back(vector<int>(m + 1, 0));
        }
        place = 0;
        for (int i = 0; i < b_compressed.size(); i++) {
            place += b_compressed[i].second;
            cols[place] = data.size();
            data.push_back(vector<int>(n + 1, 0));
        }
    }

    int get(const int i, const int j) const {
        if (rows[i] != -1) {
            return data[rows[i]][j];
        }
        if (cols[j] != -1) {
            return data[cols[j]][i];
        }
        cout << "Error: Invalid indices: get" << i << ' ' << j << endl;
        exit(-1);
    }

    void set(const int i, const int j, int value) {
        if (rows[i] != -1) {
            data[rows[i]][j] = value;
            return;
        }
        if (cols[j] != -1) {
            data[cols[j]][i] = value;
            return;
        }
        cout << "Error: Invalid indices: set" << i << ' ' << j << endl;
        exit(-1);
    }
};
#endif
