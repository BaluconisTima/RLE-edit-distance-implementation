#ifndef CHEN_CHA0_ADVANCE_C
#define CHEN_CHA0_ADVANCE_C
#include <bits/stdc++.h>
#include "../structures/GeometricView.cpp"
#define float long double

using namespace std;

GeometricView calculateOutMismatch(GeometricView &left, GeometricView &top) {
    bool is_rotated = false;

    if (left.points.back().first > top.points.back().first) {
        swap(left, top);
        reverse(left);
        reverse(top);
        is_rotated = true;
    }

    int h = left.points.back().first + 1,
            w = top.points.back().first + 1;
    GeometricView left_h = CSWM_Slide(left, h),
            top_h = CSWM_Slide(top, h);

    GeometricView left_2, top_2;
    auto split_l = splitIncluding(left_h, h - 1);
    for (auto to: split_l.first.points) {
        left_2.push_back({to.first, to.second + to.first});
    }
    for (auto to: split_l.second.points) {
        left_2.push_back({to.first + w - h, to.second + w - 1});
    }
    auto split_t = splitIncluding(top_h, w - 1);
    for (auto to: split_t.first.points) {
        top_2.push_back({to.first, to.second + h - 1});
    }
    for (auto to: split_t.second.points) {
        if (to.first == w - 1) {
            continue;
        }
        top_2.push_back({to.first, to.second - to.first - 2 + w + h});
    }


    auto out = min(left_2, top_2);

    if (is_rotated) {
        reverse(out);
    }

    return out;
}

float avg_segments_count_chen_chao = 0;

int chen_chao_advance_solution(vector<pair<char, int> > &a, vector<pair<char, int> > &b) {
    avg_segments_count_chen_chao = 0;
    GeometricView ED_left[2][b.size() + 1], ED_top[2][b.size() + 1];
    int sum = 0;
    for (int j = 0; j < b.size(); j++) {
        ED_top[0][j] = GeometricView();
        ED_top[0][j].push_back({0, sum});
        sum += b[j].second;
        ED_top[0][j].push_back({b[j].second, sum});
    }
    sum = 0;

    for (int i = 0; i < a.size(); i++) {
        ED_left[i%2][0] = GeometricView();
        ED_left[i%2][0].push_back({0, sum});
        sum += a[i].second;
        ED_left[i%2][0].push_back({a[i].second, sum});

        for (int j = 0; j < b.size(); j++) {
            GeometricView left = ED_left[i%2][j], top = ED_top[i%2][j], out;
            reverse(left);
            if (a[i].first == b[j].first) {
                // Match block
                out = left;
                for (auto to: top.points) {
                    if (to.first == 0) continue;
                    out.push_back({to.first + a[i].second, to.second});
                }
            } else {
                // Mismatch block

                out = calculateOutMismatch(left, top);
            }

            avg_segments_count_chen_chao += out.points.size();

            auto out_split = splitIncluding(out, b[j].second);

            ED_top[(i + 1)%2][j] = out_split.first;

            out_split.second.moveX(-out_split.second.points[0].first);
            ED_left[i%2][j + 1] = out_split.second;
            reverse(ED_left[i%2][j + 1]);

            ED_left[i%2][j].points.clear();
            ED_top[i%2][j].points.clear();
            left.points.clear();
            top.points.clear();
        }
    }
    int ans = ED_top[a.size()%2][b.size() - 1].points.back().second;

    avg_segments_count_chen_chao /= (a.size() * b.size());
    return ans;
}
#endif
