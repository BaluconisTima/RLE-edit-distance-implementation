#ifndef MINQUEUE_H
#define MINQUEUE_H
#include <bits/stdc++.h>
using namespace std;

struct MinQueue {
    stack<pair<int, int> > s1, s2;

    MinQueue() = default;

    void push(int value) {
        if (s1.empty()) {
            s1.push({value, value});
        } else {
            s1.push({value, min(value, s1.top().second)});
        }
    }

    void pop() {
        if (s2.empty()) {
            while (!s1.empty()) {
                int value = s1.top().first;
                s1.pop();
                if (s2.empty()) {
                    s2.push({value, value});
                } else {
                    s2.push({value, min(value, s2.top().second)});
                }
            }
        }
        return s2.pop();
    }

    int get_min() const {
        int mi = INT_MAX;
        if (!s1.empty()) {
            mi = s1.top().second;
        }
        if (!s2.empty()) {
            mi = min(mi, s2.top().second);
        }
        return mi;
    }

    size_t size() const {
        return s1.size() + s2.size();
    }
};
#endif
