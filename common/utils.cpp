#ifndef UTILS_COMMON
#define UTILS_COMMON

#include <bits/stdc++.h>
#include "../structures/GeometricView.cpp"
using namespace std;

vector<pair<char, int> > compress_string(string &original_string) {
    vector<pair<char, int> > compressed_string;
    char current_char = original_string[0];
    int count = 1;

    for (int i = 1; i < original_string.size(); i++) {
        if (original_string[i] == current_char) {
            count++;
        } else {
            compressed_string.push_back({current_char, count});
            current_char = original_string[i];
            count = 1;
        }
    }
    compressed_string.push_back({current_char, count});

    return compressed_string;
}

string decompress_string(vector<pair<char, int> > &compressed_string) {
    string decompressed_string;
    for (const auto &p: compressed_string) {
        decompressed_string += string(p.second, p.first);
    }
    return decompressed_string;
}

GeometricView convert_to_geometric(vector<int> &input) {
    GeometricView geometric;
    geometric.push_back({0, input[0]});
    for (int i = 1; i < input.size(); i++) {
        geometric.push_back({i, input[i]});
    }
    return geometric;
}

vector<int> convert_to_regular(GeometricView geometric) {
    vector<int> regular;
    regular.push_back(geometric.points[0].second);
    for (int i = 1; i < geometric.points.size(); i++) {
        int h = regular.back();
        for (int j = 0; j < geometric.points[i].first - geometric.points[i - 1].first; j++) {
            if (geometric.points[i].second < geometric.points[i - 1].second) {
                h--;
            } else if (geometric.points[i].second > geometric.points[i - 1].second) {
                h++;
            }
            regular.push_back(h);
        }
    }
    return regular;
}


float value_in_m(float ax, float ay, float bx, float by, float m) {
    if (bx - ax <= 0.000001) {
        return ay;
    }
    float k = (by - ay) / (bx - ax);
    return ay + k * (m - ax);
}

int line_type(float lx, float ly, float rx, float ry) {
    // -1 if decreasing, 1 if increasing, 0 if horizontal
    if (abs(ly - ry) < 0.00001) {
        return 0; // horizontal line
    }
    if (ly < ry) {
        return 1; // increasing line
    }
    return -1; // decreasing line
}
#endif //UTILS_COMMON
