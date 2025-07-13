#include <iostream>
#include <vector>
#include <utility>
#include "../solutions/near_optimal.cpp"
using namespace std;


void generate_hard_test(int n, int check_count, int blocks_per_add, int block_size) {
    vector<pair<char, int>> a, b;
    for (int i = 0; i < n; i++) {
        vector<pair<char, int>> a1, b1;
        float ans = 0;
        for (int j = 0; j < check_count; j++) {
            for (int q = 0; q < blocks_per_add; q++) {
                a.push_back({rand() % 100 + 1, rand() % block_size + 1});
                b.push_back({rand() % 100 + 1, rand() % block_size + 1});
            }
            near_optimal_solution(a, b);
            if (ans < avg_segments_count) {
                ans = avg_segments_count;
                a1 = a;
                b1 = b;
            }
            for (int q = 0; q < blocks_per_add; q++) {
                a.pop_back();
                b.pop_back();
            }
        }
        cerr << a.size() << ' ' << ans << endl;


       a = a1;
       b = b1;
    }
}

vector<pair<char, int> > compressed_string_generator(int block_number_limit, int block_size_limit, int char_limit_l, int char_limit_r) {
    int n = rand() % block_number_limit + 1;
    vector<pair<char, int> > compressed_string;
    for (int i = 0; i < n; i++) {
        int rnd = rand() % (char_limit_r - char_limit_l + 1) + char_limit_l;
        char c = 'a' + rnd;
        int block_size = rand() % block_size_limit + 1;
        compressed_string.push_back({c, block_size});
    }
    return compressed_string;
}


vector<pair<char, int> > compressed_string_generator(int block_number_limit, int block_size_limit, int char_limit) {
    return compressed_string_generator(block_number_limit, block_size_limit, 0, char_limit);
}


using callback_edit_distance = function<int(vector<pair<char, int> >&, vector<pair<char, int>> &)>;


void test_compare(callback_edit_distance edit_distance1, callback_edit_distance edit_distance2,
                  vector<pair<char, int>> a, vector<pair<char, int>> b) {
    int result1 = edit_distance1(a, b);
    int result2 = edit_distance2(a, b);
    if (result1 != result2) {
        cerr << "Error: Different results for the same input!";
        cerr << "Result 1: " << result1 << ", Result 2: " << result2 << endl;
        exit(-1);
    } else {
        cerr << "Both algorithms returned the same result: " << result1 << endl;
    }
}

void test_edit_distance(callback_edit_distance edit_distance, vector<pair<char, int>> a, vector<pair<char, int>> b, int test_times = 100) {
    float min_time = 100000000.0, max_time = -min_time, avg_time = 0.0;
    for (int i = 0; i < test_times; i++) {
        cerr << "Test " << i + 1 << " of " << test_times << "...\n";
        int c = clock();
        int result = edit_distance(a, b);
        c = clock() - c;
        float time = (float)c / CLOCKS_PER_SEC;
        if (time < min_time) {
            min_time = time;
        }
        if (time > max_time) {
            max_time = time;
        }
        avg_time += time;
    }
    avg_time /= test_times;
    cout << "Tested " << test_times << " times.\n";
    cout << "Min time: " << min_time << " sec\n";
    cout << "Max time: " << max_time << " sec\n";
    cout << "Avg time: " << avg_time << " sec\n";
}