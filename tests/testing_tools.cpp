#include <iostream>
#include <vector>
#include <utility>
#include "../solutions/near_optimal.cpp"
#include "../solutions/chen_chao_advance.cpp"
using namespace std;

void generate_hard_test(int n, int check_count, int blocks_per_add, int block_size) {
    vector<pair<char, int>> a, b;
    float ans1 = 0;
    for (int i = 0; i < n; i++) {
        vector<pair<char, int>> a1, b1;
        float ans = 0;
        while (ans1 + 0.1 >= ans) {
            for (int q = 0; q < blocks_per_add; q++) {
                a.push_back({rand() % 26 + 1, rand() % block_size + 1});
                b.push_back({rand() % 26 + 1, rand() % block_size + 1});
            }
            chen_chao_advance_solution(a, b);
            if (ans < avg_segments_count_chen_chao) {
                ans = avg_segments_count_chen_chao;
                a1 = a;
                b1 = b;
            }
            for (int q = 0; q < blocks_per_add; q++) {
                a.pop_back();
                b.pop_back();
            }
        }
        ans1 = ans;
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
double rnd() {
     int r = rand() % 10000;
    return (double) r / (double) 10000;
}

void generate_hard_test2(int n, int blocks_change_base, int sizes_cap) {
    auto a = compressed_string_generator(n, sizes_cap, 0, 5),
         b = compressed_string_generator(10, sizes_cap, 0, 5);
    float temp = 1;
    float last_ans = 0;
    while (last_ans < 15) {
        temp *= 0.99;
        int changes = blocks_change_base * temp;
        changes = rand() % (changes + 1) + 1;
        auto a1 = a, b1 = b;
        while (changes--) {
            if (rand() % 2 == 0) {
                int index = rand() % a1.size();
                a1[index].first = 'a' + rand() % 5;
                a1[index].second = 1000 + rand() % sizes_cap + 1;
            } else {
                int index = rand() % b1.size();
                b1[index].first = 'a' + rand() % 5;
                b1[index].second = rand() % sizes_cap + 1;
            }
        }
        int ans = chen_chao_advance_solution(a1, b1);
        float new_ans = avg_segments_count_chen_chao;
        if (new_ans > last_ans || rnd() < exp((new_ans - last_ans) / temp)) {
            cerr << new_ans << endl;
            last_ans = new_ans;
            a = a1;
            b = b1;
        }
    }
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