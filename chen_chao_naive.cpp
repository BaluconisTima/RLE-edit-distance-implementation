#ifndef CHEN_CHAO_NAIVE_C
#define CHEN_CHAO_NAIVE_C
/// O (N * m + M * n) solution for the edit distance problem
#include <bits/stdc++.h>
#include "common_utils.cpp"
#include "gridTable.cpp"
#include "minQueue.cpp"

using namespace std;

vector<int> SWM(vector<int> a, int w) {
    MinQueue q;
    vector<int> result;
    for (int i = 0; i < a.size() + w - 1; i++) {
        if (i >= a.size()) q.push(INT_MAX);
        else
            q.push(a[i]);
        if (q.size() > w) {
            q.pop();
        }
        result.push_back(q.get_min());
    }

    return result;
}

vector<int> calculateOutMismatch(vector<int> left, vector<int> top) {
    bool is_rotated = false;

    if (left.size() > top.size()) {
        swap(left, top);
        reverse(left.begin(), left.end());
        reverse(top.begin(), top.end());
        is_rotated = true;
    }
    vector<int> left_h = SWM(left, left.size()),
            top_h = SWM(top, left.size());

    vector out(left.size() + top.size() - 1, INT_MAX);
    for (int i = 0; i < left.size(); i++) {
        out[i] = left_h[i] + i;
    }
    for (int i = left.size() - 1; i < top.size(); i++) {
        out[i] = left_h[left.size() - 1] + i;
    }
    for (int i = top.size(); i < out.size(); i++) {
        out[i] = left_h[i - top.size() + left.size()] + top.size() - 1;
    }

    for (int i = 0; i < top.size(); i++) {
        out[i] = min(out[i], ((int) left.size() + top_h[i] - 1));
    }

    for (int i = top.size(); i < out.size(); i++) {
        out[i] = min(out[i], ((int) left.size() + (int) top.size() + top_h[i] - i - 2));
    }

    if (is_rotated) {
        reverse(out.begin(), out.end());
    }
    return out;
}

int chen_chao_naive_solution(vector<pair<char, int> > &a, vector<pair<char, int> > &b) {
    const string a_decompressed = decompress_string(a);
    const string b_decompressed = decompress_string(b);

    gridTable ED(a, a_decompressed.size(), b, b_decompressed.size());
    for (int i = 0; i <= a_decompressed.size(); i++) {
        ED.set(i, 0, i);
    }
    for (int j = 0; j <= b_decompressed.size(); j++) {
        ED.set(0, j, j);
    }
    int place_i = 0;
    for (int i = 0; i < a.size(); i++) {
        int place_j = 0;
        for (int j = 0; j < b.size(); j++) {
            if (a[i].first == b[j].first) {
                // Match block
                for (int q = place_j + 1; q <= place_j + b[j].second; q++) {
                    int diagonalDistance = min(a[i].second, q - place_j);
                    ED.set(place_i + a[i].second, q,
                           ED.get(place_i + a[i].second - diagonalDistance, q - diagonalDistance));
                }
                for (int q = place_i + 1; q <= place_i + a[i].second; q++) {
                    int diagonalDistance = min(b[j].second, q - place_i);
                    ED.set(q, place_j + b[j].second,
                           ED.get(q - diagonalDistance, place_j + b[j].second - diagonalDistance));
                }
            } else {
                // Mismatch block
                vector<int> left, top;
                for (int q = place_i + a[i].second; q >= place_i; q--) {
                    left.push_back(ED.get(q, place_j));
                }
                for (int q = place_j; q <= place_j + b[j].second; q++) {
                    top.push_back(ED.get(place_i, q));
                }
                vector<int> out = calculateOutMismatch(left, top);
                int place_out = 0;
                for (int q = place_j; q <= place_j + b[j].second; q++) {
                    ED.set(place_i + a[i].second, q, out[place_out++]);
                }
                for (int q = place_i + a[i].second - 1; q >= place_i; q--) {
                    ED.set(q, place_j + b[j].second, out[place_out++]);
                }
            }
            place_j += b[j].second;
        }
        place_i += a[i].second;
    }

    return ED.get(a_decompressed.size(), b_decompressed.size());
}
#endif
