#include <bits/stdc++.h>
#include "solutions/chen_chao_naive.cpp"
#include "solutions/chen_chao_advance.cpp"
#include "solutions/naive_dp.cpp"
#include "solutions/near_optimal.cpp"
#include "tests/testing_tools.cpp"
using namespace std;

int main() {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    srand(time(0));
    generate_hard_test2(100, 10, 1000);

    //generate_hard_test(100, 1000, 10, 1000);
    // int n_limit = 10000, m_limit = 1000, block_limit = 10000;
    // // while (true) {
    // //     auto a = compressed_string_generator(n_limit, block_limit, 0, 10),
    // //          b = compressed_string_generator(m_limit, block_limit, 0, 10);
    // //     test_compare(chen_chao_naive_solution, near_optimal_solution, a, b);
    // //
    // // }
    // auto a = compressed_string_generator(n_limit, block_limit, 5, 10),
    //      b = compressed_string_generator(m_limit, block_limit, 0, 5);
    //
    // //reverse(a.begin(), a.end());
    //
    // test_edit_distance(near_optimal_solution, a, b, 1);
    //
    // test_edit_distance(chen_chao_advance_solution, a, b, 1);
}
