#ifndef NEAR_OPTIMAL_SOLUTION
#define NEAR_OPTIMAL_SOLUTION

#include <bits/stdc++.h>
#include "../structures/BST/BstView.cpp"

using namespace std;

node *calculateOutMismatch(node *left, node *top) {
    int h = left->get_rightmost()->get_xr() + 1,
            w = top->get_rightmost()->get_xr() + 1;

    if (w < h) {
        auto top_w = SWM(top, w),
                left_w = SWM(left, w);

        auto split_t = smart_split(top_w, w - 1);
        float topww = split_t.first->get_rightmost()->get_yr();
        split_t.first->add_delta_root(0, h - 1);
        auto intermission = new node(w - 1, h - 1, topww + h - 1, topww + w - 1);
        split_t.second->add_delta_root(h - w, w + h - 2);
        split_t.second->move_gradient_root(-1);
        split_t.second->update_types();
        auto top_2 = smart_merge(split_t.first, intermission);
        top_2 = smart_merge(top_2, split_t.second);

        auto split_l = smart_split(left_w, w - 1);
        split_l.first->move_gradient_root(1);
        split_l.first->update_types();
        split_l.second->add_delta_root(0, w - 1);
        auto left_2 = smart_merge(split_l.first, split_l.second);

        return combine(top_2, left_2);
    } else {
        types_updated_counter = 0;
        auto left_h = SWM(left, h),
                top_h = SWM(top, h);
        node *left_2;
        auto split_l = smart_split(left_h, h - 1);
        float lefthh = split_l.first->get_rightmost()->get_yr();

        auto intermission = new node(h - 1, w - 1, lefthh + h - 1, lefthh + w - 1);

        split_l.first->move_gradient_root(1);
        split_l.second->add_delta_root(w - h, w - 1);
        left_2 = smart_merge(split_l.first, intermission);
        left_2 = smart_merge(left_2, split_l.second);

        auto split_t = smart_split(top_h, w - 1);
        split_t.first->add_delta_root(0, h - 1);
        split_t.second->move_gradient_root(-1);
        split_t.second->add_delta_root(0, -2 + w + h);

        auto top_2 = smart_merge(split_t.first, split_t.second);
        return combine(top_2, left_2);
    }
}

float avg_segments_count = 0;

int near_optimal_solution(vector<pair<char, int> > &a, vector<pair<char, int> > &b) {
    avg_segments_count = 0;
    node *ED_left[2][b.size() + 1];
    node *ED_top[2][b.size() + 1];
    int sum = 0;
    for (int j = 0; j < b.size(); j++) {
        ED_top[0][j] = new node(0, b[j].second, sum, sum + b[j].second);
        sum += b[j].second;
    }
    sum = 0;
    types_updated_counter = 0;
    for (int i = 0; i < a.size(); i++) {
        ED_left[i%2][0] = new node(0, a[i].second, sum + a[i].second, sum);
        sum += a[i].second;
        for (int j = 0; j < b.size(); j++) {
            auto left = ED_left[i%2][j];
            auto top = ED_top[i%2][j];
            node *out;
            if (a[i].first == b[j].first) {
                // Match block;
                top->add_delta_root(left->get_rightmost()->get_xr(), 0);
                out = smart_merge(left, top);
            } else {
                // Mismatch block
                out = calculateOutMismatch(left, top);
            }
            out->pull();
            avg_segments_count += out->sz;
            auto out_split = smart_split(out, b[j].second);
            out_split.second->add_delta_root(-out_split.second->get_leftmost()->get_xl(), 0);

            ED_top[(i + 1)%2][j] = out_split.first;
            ED_left[i%2][j + 1] = out_split.second;
        }
    }
    avg_segments_count = avg_segments_count / (b.size()) / (a.size());
    return ED_top[a.size()%2][b.size() - 1]->get_rightmost()->get_yr();
}

#endif // NEAR_OPTIMAL_SOLUTION
