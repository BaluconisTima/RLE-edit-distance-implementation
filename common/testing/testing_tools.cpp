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
    int n = rand() % block_number_limit + 1;
    //int n = block_number_limit;
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


pair<vector<pair<char, int>>, vector<pair<char, int>>> null_test_case() {
    return {{}, {}};
}

pair<vector<pair<char, int>>, vector<pair<char, int>>> read_test_in(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
       return null_test_case();
    }

    int n, m;
    file >> n >> m;

    vector<pair<char, int>> a(n), b(m);
    for (int i = 0; i < n; i++) {
        char c;
        int size;
        file >> c >> size;
        a[i] = {c, size};
    }
    for (int i = 0; i < m; i++) {
        char c;
        int size;
        file >> c >> size;
        b[i] = {c, size};
    }

    return {a, b};
}

int read_test_out(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open output file " << filename << endl;
        return -1;
    }
    int result;
    file >> result;
    return result;
}

void update_tests_answer(string test_folder) {
    int total_test_cases = 0;
    while (true) {
        total_test_cases++;
        if (!ifstream(test_folder + "test" + to_string(total_test_cases) + ".in")) {
            break;
        }
        auto test = read_test_in(test_folder + "test" + to_string(total_test_cases) + ".in");
        auto a = test.first, b = test.second;
        ofstream file((test_folder + "test" + to_string(total_test_cases) + ".out").c_str());
        cerr << "Test #" << total_test_cases << endl;
        int ans = near_optimal_solution(a, b);
        file << ans << '\n';
    }
}


void test_folder_ms(string test_folder) {
    int total_test_cases = 0;
    int worst_time_ms = 0, best_time_ms = 1000000000, avg_time_ms = 0;
    while (true) {
        total_test_cases++;
        auto test = read_test_in(test_folder + "test" + to_string(total_test_cases) + ".in");
        if (test == null_test_case()) {
            break;
        }
        auto a = test.first, b = test.second;
        auto c = clock();
        int answ = near_optimal_solution(a, b);
        c = clock() - c;
        int time_ms = (int)(c * 1000 / CLOCKS_PER_SEC);
        worst_time_ms = max(worst_time_ms, time_ms);
        best_time_ms = min(best_time_ms, time_ms);
        avg_time_ms += time_ms;
        cerr << "Test #" << total_test_cases << ": " << answ << " in " << time_ms << " ms" << endl;
    }
    avg_time_ms /= total_test_cases - 1;
    cerr << "Total test cases: " << total_test_cases - 1 << endl;
    cerr << "Worst time: " << worst_time_ms << " ms" << endl;
    cerr << "Best time: " << best_time_ms << " ms" << endl;
    cerr << "Average time: " << avg_time_ms << " ms" << endl;
}

void test_folder_ms_decomressed_optimistic(string test_folder) {
    int total_test_cases = 0;
    int worst_time_ms = 0, best_time_ms = 1000000000, avg_time_ms = 0;
    int total_good_test_cases = 0;
    while (true) {
        total_test_cases++;
        auto test = read_test_in(test_folder + "test" + to_string(total_test_cases) + ".in");
        if (test == null_test_case()) {
            break;
        }
        auto a = test.first, b = test.second;
        string a_decompressed = decompress_string(a),
               b_decompressed = decompress_string(b);
        int n = a.size(), m = b.size();
        int n2 = a_decompressed.size() + b_decompressed.size();
        if (n * 10 <= n2) {
            continue;
        }
        total_good_test_cases++;
        auto c = clock();
        int answ = chen_chao_naive_solution(a, b);
        c = clock() - c;
        int time_ms = (int)(c * 1000 / CLOCKS_PER_SEC);
        worst_time_ms = max(worst_time_ms, time_ms);
        best_time_ms = min(best_time_ms, time_ms);
        avg_time_ms += time_ms;
        cerr << "Test #" << total_test_cases << ": " << answ << " in " << time_ms << " ms" << endl;
    }
    avg_time_ms /= total_good_test_cases;
    cerr << "Total good test cases: " << total_good_test_cases << endl;
    cerr << "Total test cases: " << total_test_cases - 1 << endl;
    cerr << "Worst time: " << worst_time_ms << " ms" << endl;
    cerr << "Best time: " << best_time_ms << " ms" << endl;
    cerr << "Average time: " << avg_time_ms << " ms" << endl;
}

void test_folder_answers(string test_folder) {
    int total_test_cases = 0;
    while (true) {
        total_test_cases++;
        cerr << "Test #" << total_test_cases << endl;

        auto test = read_test_in(test_folder + "test" + to_string(total_test_cases) + ".in");
        if (test == null_test_case()) {
            break;
        }
        int ans = read_test_out(test_folder + "test" + to_string(total_test_cases) + ".out");
        auto a = test.first, b = test.second;
        auto ans2 = chen_chao_advance_solution(a, b);
        if (ans != ans2) {
            cerr << "Error in test #" << total_test_cases << ": expected " << ans << ", got " << ans2 << endl;
            exit(1);
        } else {
            cerr << "Test #" << total_test_cases << " passed: " << ans2 << endl;
        }

    }
}

pair<vector<pair<char, int>>, vector<pair<char, int>>> generate_hard_test2(int n1, int n2, int blocks_change_base, int sizes_cap) {
    auto a = compressed_string_generator(n1, sizes_cap, 0, 5),
         b = compressed_string_generator(n2, sizes_cap, 2, 7);
    chen_chao_advance_solution(a, b);


    for (int local_cap = 0; local_cap < 7; local_cap++) {
        cerr << "Local cap: " << local_cap << endl;
        float temp = 0.1 + 0.9/(1 + sqrt(local_cap));
        float last_ans = avg_segments_count_chen_chao;
        while (last_ans < local_cap) {
            temp *= 0.995;
            temp = max(temp, (float)0.001);
            int changes = blocks_change_base;
            changes = rand() % (changes + 1) + 1;
            auto a1 = a, b1 = b;
            while (changes--) {
                if (rand() % 2) {
                    int index = rand() % a1.size();
                    a1[index].first = 'a' + rand() % 5;
                    a1[index].second = rand() % sizes_cap + 1;
                } else {
                    int index = rand() % b1.size();
                    b1[index].first = 'a' + rand() % 5 + 2;
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
    }
    return {a, b};
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