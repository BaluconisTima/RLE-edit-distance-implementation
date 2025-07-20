#include <iostream>
#include <vector>
#include <utility>
#include "../../solutions/near_optimal.cpp"
#include "../../solutions/chen_chao_advance.cpp"
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
    // int n = rand() % block_number_limit + 1;
    int n = block_number_limit;
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
    auto a = compressed_string_generator(50, sizes_cap, 0, 3),
         b = compressed_string_generator(50, sizes_cap, 0, 3);

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

    chen_chao_advance_solution(a, b);
    float temp = 0;
    float last_ans = avg_segments_count_chen_chao;

    while (last_ans < 20) {
        //temp *= 0.99;
        int changes = blocks_change_base;
        changes = rand() % (changes + 1) + 1;
        auto a1 = a, b1 = b;
        while (changes--) {
            if (rand() % 3) {
                int index = rand() % a1.size();
                a1[index].first = 'a' + rand() % 2;
                a1[index].second = rand() % sizes_cap + 1;
            } else {
                int index = rand() % b1.size();
                b1[index].first = 'a' + rand() % 2;
                b1[index].second = rand() % sizes_cap + 1;
            }
        }
        int ans = chen_chao_advance_solution(a1, b1);
        float new_ans = avg_segments_count_chen_chao;
        if (new_ans > last_ans || rnd() < exp((new_ans - last_ans) / max((float)0.00001, temp))) {
            cerr << new_ans << endl;
            last_ans = new_ans;
            a = a1;
            b = b1;
        }
    }
    cerr << "a = {\n";
    for (auto &p: a) {
        cerr << "    {'" << p.first << "', " << p.second << "},\n";
    }
    cerr << "};\n";
    cerr << "b = {\n";
    for (auto &p: b) {
        cerr << "    {'" << p.first << "', " << p.second << "},\n";
    }
    cerr << "};\n";
}



vector<pair<char, int> > compressed_string_generator(int block_number_limit, int block_size_limit, int char_limit) {
    return compressed_string_generator(block_number_limit, block_size_limit, 0, char_limit);
}


vector<pair<char, int> > generate_string_for_not_compress_algorithms(int block_number_limit, int block_size_limit, int char_limit, int total_size_limit) {
    auto a = compressed_string_generator(block_number_limit, block_size_limit, char_limit);
    int total_sz = 0;
    for (auto to: a) {
        total_sz += to.second;
    }
    while (total_sz > total_size_limit) {
        total_sz -= a.back().second;
        if (total_sz < total_size_limit) {
            a.back().second = total_size_limit - total_sz;
            break;
        }
        a.pop_back();
    }
    return a;
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