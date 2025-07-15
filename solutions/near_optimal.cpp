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
        cerr << "lh th" << endl;
        print_tree(left_h);
        print_tree(top_h);
        node *left_2;
        auto split_l = smart_split(left_h, h - 1);
        cerr << "split_l" << endl;
        print_tree(split_l.first);
        print_tree(split_l.second);


        float lefthh = split_l.first->get_rightmost()->get_yr();

        auto intermission = new node(h - 1, w - 1, lefthh + h - 1, lefthh + w - 1);
        cerr << "intermission" << endl;
        print_tree(intermission);

        split_l.first->move_gradient_root(1);
        cerr << "split_l.first" << endl;
        print_tree(split_l.first);
        split_l.second->add_delta_root(w - h, w - 1);
        cerr << "split_l.second" << endl;
        print_tree(split_l.second);
        left_2 = smart_merge(split_l.first, intermission);
        cerr << "left_2" << endl;
        print_tree(left_2);
        left_2 = smart_merge(left_2, split_l.second);
        cerr << "left_2 after merge" << endl;
        print_tree(left_2);

        auto split_t = smart_split(top_h, w - 1);
        cerr << "split_t" << endl;
        print_tree(split_t.first);
        split_t.first->add_delta_root(0, h - 1);
        cerr << "split_t.first after add_delta_root" << endl;
        print_tree(split_t.first);
        split_t.second->move_gradient_root(-1);
        cerr << "split_t.second" << endl;
        print_tree(split_t.second);
        split_t.second->add_delta_root(0, -2 + w + h);
        cerr << "split_t.second after add_delta_root" << endl;
        print_tree(split_t.second);

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

    float total_time = 0;
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
                auto c = clock();

                // Mismatch block
                out = calculateOutMismatch(left, top);
                c = clock() - c;
                float time = (float)c / CLOCKS_PER_SEC;
                total_time += time;
            }
            out->pull();
            avg_segments_count += out->sz;
            // cerr << out->sz << ' ' << types_updated_counter << endl;
            // types_updated_counter = 0;
            cerr << "Segments count: " << out->sz << endl;
            print_tree(out);
            auto out_split = smart_split(out, b[j].second);
            out_split.second->add_delta_root(-out_split.second->get_leftmost()->get_xl(), 0);

            ED_top[(i + 1)%2][j] = out_split.first;
            ED_left[i%2][j + 1] = out_split.second;
        }
    }
    cerr << "Total time: " << total_time << " seconds" << endl;
    // cerr << "Types updated: " << types_updated_counter << endl;
    // cerr << "Average segments count: " << avg_segments_count << endl;
    // cerr << "const: " << types_updated_counter / (avg_segments_count * log(avg_segments_count)) << endl;

    avg_segments_count = avg_segments_count / (b.size()) / (a.size());
    cerr << "Average segments count: " << avg_segments_count << endl;
    return ED_top[a.size()%2][b.size() - 1]->get_rightmost()->get_yr();
}

#endif // NEAR_OPTIMAL_SOLUTION
