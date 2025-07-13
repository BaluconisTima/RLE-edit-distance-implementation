#ifndef NAIVE_DP
#define NAIVE_DP
// O(N * M) solution for the edit distance problem
#include <bits/stdc++.h>
#include "../common/utils.cpp"

using namespace std;

int naive_dp_solution(vector<pair<char, int>>& a_compressed, vector<pair<char, int>>& b_compressed) {
    const string a = decompress_string(a_compressed);
    const string b = decompress_string(b_compressed);

    vector ED(a.size() + 1, vector(b.size() + 1, 0));
    for (int i = 0; i <= a.size(); i++) {
        ED[i][0] = i;
    }
    for (int j = 0; j <= b.size(); j++) {
        ED[0][j] = j;
    }
    for (int i = 1; i <= a.size(); i++) {
        for (int j = 1; j <= b.size(); j++) {
            ED[i][j] = min({ED[i - 1][j] + 1,
                              ED[i][j - 1] + 1,
                              ED[i - 1][j - 1] + (a[i - 1] != b[j - 1])});
        }
    }

    return ED[a.size()][b.size()];
}
#endif
