#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>
#include "../structures/GeometricView.cpp"
#include "../common/utils.cpp"
using namespace std;

GeometricView generate_point_test(int size_t, int block_limit) {
    int n = rand() % size_t + 1;
    GeometricView gv;
    pair<int, int> now = {0, rand() % block_limit + 1 + size_t * block_limit * 2};
    gv.push_back(now);
    vector<int> points = {-1, 0, 1};
    for (int i = 0; i < n; i++) {
        int j = rand() % 3;
        int x = rand() % block_limit + 1;
        now = {now.first + x, now.second + points[j] * x};
        gv.push_back(now);
    }
    return gv;
}
