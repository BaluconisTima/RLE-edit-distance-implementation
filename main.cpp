#include <bits/stdc++.h>
#include "solutions/chen_chao_naive.cpp"
#include "solutions/chen_chao_advance.cpp"
#include "solutions/naive_dp.cpp"
#include "solutions/near_optimal.cpp"
#include "tests/testing_tools.cpp"
using namespace std;
#define float long double

int main() {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    srand(time(0));
    //generate_hard_test2(100, 10, 1000);

    //generate_hard_test(100, 1000, 10, 1000);
    int n_limit = 100, m_limit = 100, block_limit = 1000;
    auto a = compressed_string_generator(n_limit, block_limit, 5, 10),
 b = compressed_string_generator(m_limit, block_limit, 0, 5);
          a = {
        {'a', 185},
        {'e', 928},
        {'b', 150},
        {'e', 258},
        {'b', 119},
        {'e', 109},
        {'e', 85},
        {'b', 84},
        {'e', 59},
        {'a', 134},
        {'b', 1},
        {'e', 81},
        {'a', 600},
        {'e', 335},
        {'a', 180},
        {'e', 234},
        {'a', 31},
        {'e', 434},
        {'a', 724},
        {'b', 71},
        {'e', 303},
        {'e', 409},
        {'a', 34},
        {'a', 124},
        {'b', 25},
        {'e', 306},
        {'b', 108},
        {'a', 37},
        {'b', 4},
        {'e', 559},
        {'a', 12},
        {'b', 9},
        {'a', 118},
        {'b', 88},
        {'a', 68},
        {'e', 74},
        {'c', 16},
        {'b', 53},
        {'a', 4},
        {'e', 21},
        {'a', 5},
        {'b', 5},
        {'a', 21},
        {'b', 8},
        {'a', 76},
        {'b', 6},
        {'e', 1},
        {'b', 65},
        {'a', 66},
        {'e', 59},
        {'b', 13},
        {'b', 8},
        {'a', 3},
        {'b', 2},
        {'a', 33},
        {'b', 7},
        {'a', 34},
        {'d', 736},
        {'a', 3},
        {'d', 875},
        {'c', 711},
        {'d', 622},
        {'d', 591},
        {'e', 201},
        {'a', 18},
        {'b', 37},
        {'a', 61},
        {'b', 2},
        {'a', 33},
        {'b', 1},
        {'a', 68},
        {'b', 43},
        {'a', 9},
        {'a', 11},
        {'b', 2},
        {'e', 117},
        {'a', 27},
        {'a', 3},
        {'b', 7},
        {'a', 36},
        {'b', 21},
        {'a', 140},
        {'e', 139},
        {'b', 121},
        {'b', 20},
        {'e', 10},
        {'b', 9},
        {'b', 78},
        {'a', 11},
        {'b', 7},
        {'b', 16},
        {'a', 44},
        {'b', 25},
        {'a', 16},
        {'d', 1},
        {'a', 20},
        {'a', 18},
        {'b', 13},
        {'a', 65},
        {'a', 956},
    };
    b = {
        {'b', 949},
        {'e', 724},
        {'b', 996},
        {'a', 998},
        {'a', 999},
    };
    while (true) {
        test_compare(chen_chao_advance_solution, near_optimal_solution, a, b);
        test_edit_distance(near_optimal_solution, a, b, 1);
        test_edit_distance(chen_chao_advance_solution, a, b, 1);

        a = compressed_string_generator(n_limit, block_limit, 0, 10),
             b = compressed_string_generator(m_limit, block_limit, 0, 10);

    }

    // int a_sz_orig = a.size(), b_sz_orig = b.size();
    // // for (int i = 0; i < 1000; i++) {
    // //     a.push_back(a[i % a_sz_orig]);
    // // }
    // // for (int i = 0; i < 1000; i++) {
    // //     b.push_back(b[i % b_sz_orig]);
    // // }
    // // while (a.size() < 1000) {
    // //     a.push_back({'a', 1000});
    // // }
    // // while (b.size() < 400) {
    // //     b.push_back({'b', rand() % 100 + 1});
    // // }
    // //
    // // //reverse(a.begin(), a.end());
    // //
    // test_edit_distance(near_optimal_solution, a, b, 1);
    //
    // test_edit_distance(chen_chao_advance_solution, a, b, 1);
}
