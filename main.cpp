#include <bits/stdc++.h>
#include "chen_chao_naive.cpp"
#include "chen_chao_advance.cpp"
#include "naive_dp.cpp"
//#include "GeometricView.cpp"
#include "near_optimal_solution.cpp"
#include "common_utils.cpp"
using namespace std;

GeometricView generate_point_test(int size_t, int block_limit) {
    int n = rand() % size_t + 1;
    GeometricView gv;
    pair<int, int> now = {0, rand() % block_limit + 1 + size_t * block_limit * 2};
    gv.push_back(now);
    vector<int> points = {-1, 0, 1};
    for (int i = 0; i < n; i++) {
        int j = rand() % 3;
        int x = rand() % block_limit + 1;
        now = {now.first + x, now.second + points[j] * x};
        gv.push_back(now);
    }
    return gv;
}

void test_geometric_SWM(int size_t, int block_limit) {
    GeometricView gv;
    gv.push_back({0, 0});
    gv.push_back({3, 3});
    gv.push_back({6, 0});
    gv = generate_point_test(size_t, block_limit);

    // cout << "--------------------------" << endl;
    for (int i = 1; i <= 2 * gv.points.back().first; i++) {
        //for (int i = 5; i <= 5; i++) {
        GeometricView cswm = CSWM_Slide(gv, i);
        vector<int> regular = convert_to_regular(gv);
        vector<int> regular_SWM = convert_to_regular(cswm);
        vector<int> regular_SWM_naive = SWM(regular, i);

        if (regular_SWM != regular_SWM_naive) {
            cout << "Error: Test case failed." << endl;
            cout << "--------------------------" << endl;
            for (auto to: gv.points) {
                cout << to.first << " " << to.second << endl;
            }
            cout << "i: " << i << endl;
            cout << "SLIDE SWM: ";
            for (const auto &p: regular_SWM) {
                cout << p << " ";
            }
            cout << endl;
            cout << "Naive SWM: ";
            for (const auto &p: regular_SWM_naive) {
                cout << p << " ";
            }
            cout << endl;
            exit(-1);
        } else {
            //cout << "Test case passed." << endl;
        }
    }
}

vector<pair<char, int> > compressed_string_generator(int size_limit, int block_limit, int char_limit) {
    int n = rand() % size_limit + 1;
    vector<pair<char, int> > compressed_string;
    for (int i = 0; i < n; i++) {
        char c = 'a' + (rand() % char_limit);
        int block_size = rand() % block_limit + 1;
        compressed_string.push_back({c, block_size});
    }
    return compressed_string;
}

void test_near_optimal_solution(int size_t, int block_limit) {
    vector<pair<char, int> > a = compressed_string_generator(size_t, block_limit, 26),
            b = compressed_string_generator(size_t, block_limit, 26);
    //     vector<pair<char, int> > a = {{'d',5},{'x',33},{'b',42},{'i',82},{'g',70},{'e',92},{'f',22},{'d',100},{'y',52},{'j',24},{'o',87},{'k',41},{'s',43},{'u',59},{'h',87},{'s',6},{'s',3},{'u',27},{'b',96},{'x',22},{'h',46},{'k',57},{'j',54},{'h',65},{'i',52},{'h',75},},
    // b = {{'q',100},{'o',74},{'p',98},{'b',96},{'m',77},{'r',83},{'c',17},{'x',47},{'y',33},{'x',20},{'h',25},{'t',84},{'h',61},{'e',10},{'g',72},{'i',78},{'o',29},{'b',30},{'h',41},{'i',12},};
    cerr << "--------------------------" << endl;
    // cout << "A: ";
    // for (const auto& p : a) {
    //     cout << p.first << " " << p.second << '\n';
    // }
    // cout << endl;
    // cout << "B: ";
    // for (const auto& p : b) {
    //     cout << p.first << " " << p.second << '\n';
    // }
    // cout << endl;
    cerr << "Test case: " << endl;
    cerr << "a = {";
    for (const auto &p: a) {
        cerr << "{\'" << p.first << "\'," << p.second << "},";
    }
    cerr << "}," << endl;
    cerr << "b = {";
    for (const auto &p: b) {
        cerr << "{\'" << p.first << "\'," << p.second << "},";
    }
    cerr << "}" << endl;

    int chen_chao_answer = chen_chao_advance_solution(a, b);
    cerr << "Chen Chao answer: " << chen_chao_answer << endl;
    int near_optimal_answer = near_optimal_solution(a, b);
    cerr << "Near optimal answer: " << near_optimal_answer << endl;
    if (chen_chao_answer != near_optimal_answer) {
        cout << "Error: Test case failed." << endl;
        cout << chen_chao_answer << " " << near_optimal_answer << endl;
        exit(-1);
    } else {
        //cout << "Test case passed." << endl;
    }
}

int main() {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    srand(time(0));

    for (int i = 0; i < 100000; i++) {
        test_near_optimal_solution(100, 1000);
    }
    //
    // for (int i = 0; i < 1000; i++) {
    //     //cerr << "----------- Test case " << i << " -----------" << endl;
    //     auto a = compressed_string_generator(50, 50, 5),
    //          b = compressed_string_generator(50, 50, 5);
    //     cout << "Test case " << i << endl;
    //     int dp_answer = naive_dp_solution(a, b);
    //     int chen_chao_answer = chen_chao_advance_solution(a, b);
    //     if (dp_answer != chen_chao_answer) {
    //         cout << "Error: Test case " << i << " failed." << endl;
    //         cout << dp_answer << " " << chen_chao_answer << endl;
    //         cout << "A: ";
    //         for (const auto& p : a) {
    //             cout << p.first << " " << p.second << '\n';
    //         }
    //         cout << endl;
    //         cout << "B: ";
    //         for (const auto& p : b) {
    //             cout << p.first << " " << p.second << '\n';
    //         }
    //         cout << endl;
    //         exit(-1);
    //     }
    //
    // }
}
