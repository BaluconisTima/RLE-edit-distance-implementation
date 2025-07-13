#ifndef GEOMETRIC_VIEW_COMMON
#define GEOMETRIC_VIEW_COMMON

#include <bits/stdc++.h>
using namespace std;

bool check_on_line(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    return (b.first - a.first) * (c.second - a.second) == (b.second - a.second) * (c.first - a.first);
}

pair<int, int> lines_intersection_by_points(pair<int, int> a, pair<int, int> b, pair<int, int> c, pair<int, int> d) {
    long long x1 = a.first, y1 = a.second;
    long long x2 = b.first, y2 = b.second;
    long long x3 = c.first, y3 = c.second;
    long long x4 = d.first, y4 = d.second;

    long long d1 = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d1 == 0) {
        return {INT_MAX, INT_MAX};
    }

    long long x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d1;
    long long y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d1;
    return {x, y};
}

int in_point_value(pair<int, int> a, pair<int, int> b, int x) {
    int d = (b.second - a.second) / (b.first - a.first);
    return a.second + d * (x - a.first);
}

struct GeometricView {
    deque<pair<int, int> > points;

    void push_back(pair<int, int> point) {
        // check if it's already on line from prev two points:
        if (points.size() > 1) {
            if (check_on_line(points[points.size() - 2], points[points.size() - 1], point)) {
                points.pop_back();
            }
        }
        points.push_back(point);
    }

    void moveX(int d) {
        for (auto &point: points) {
            point.first += d;
        }
    }
};

void reverse(GeometricView &a) {
    int flip = a.points.back().first + a.points.front().first;
    reverse(a.points.begin(), a.points.end());
    for (auto &point: a.points) {
        point.first = flip - point.first;
    }
}

pair<GeometricView, GeometricView> splitIncluding(GeometricView a, int x) {
    GeometricView right;
    while (a.points.size() > 1) {
        if (a.points.back().first >= x && a.points[a.points.size() - 2].first >= x) {
            right.push_back(a.points.back());
            a.points.pop_back();
        } else if (a.points.back().first >= x && a.points[a.points.size() - 2].first < x) {
            right.push_back(a.points.back());
            pair<int, int> interPoint = lines_intersection_by_points(a.points[a.points.size() - 2], a.points.back(),
                                                                     {x, 0}, {x, INT_MAX});
            if (interPoint != pair<int, int>{INT_MAX, INT_MAX}) {
                right.push_back(interPoint);
            } else {
                cerr << "Error splitting GeometricView: intersection point is invalid." << endl;
                exit(1);
            }
            a.points.pop_back();
            if (interPoint.first != a.points.back().first) a.push_back(interPoint);

            break;
        } else break;
    }
    reverse(right.points.begin(), right.points.end());
    return {a, right};
}

GeometricView min(GeometricView a, GeometricView b) {
    if (a.points.empty()) {
        return b;
    }
    if (b.points.empty()) {
        return a;
    }
    // cerr << "A "  << endl;
    // for (auto to: a.points) {
    //     cerr << to.first << " " << to.second << endl;
    // }
    // cerr << "B "  << endl;
    // for (auto to: b.points) {
    //     cerr << to.first << " " << to.second << endl;
    // }
    if (a.points[0].first != b.points[0].first) {
        cerr << "Error finding min: GeometricView min: points do not start at the same x-coordinate." << endl;
        exit(1);
    }
    if (a.points.back().first != b.points.back().first) {
        cerr << "Error finding min: GeometricView min: points do not end at the same x-coordinate." << endl;
        exit(1);
    }
    if (a.points[0].second > b.points[0].second) {
        swap(a, b);
    }

    GeometricView result;
    result.points.push_back(a.points[0]);
    pair<int, int> last_a = a.points[0];
    pair<int, int> last_b = b.points[0];
    a.points.pop_front();
    b.points.pop_front();
    while (a.points.size() && b.points.size()) {
        if (a.points[0].first > b.points[0].first) {
            swap(a, b);
            swap(last_a, last_b);
        }
        pair<int, int> cur_a = a.points[0],
                cur_b = {a.points[0].first, in_point_value(last_b, b.points[0], a.points[0].first)};
        if (cur_a.second <= cur_b.second && last_a.second <= last_b.second) {
            result.push_back(cur_a);
        } else if (cur_b.second <= cur_a.second && last_b.second <= last_a.second) {
            result.push_back(cur_b);
        } else {
            auto interPoint = lines_intersection_by_points(last_a, cur_a, last_b, cur_b);
            if (interPoint != pair<int, int>{INT_MAX, INT_MAX}) {
                if (interPoint.first > result.points.back().first) {
                    result.push_back(interPoint);
                }
                interPoint.first++;
                if (interPoint.first < cur_a.first && interPoint.first <= cur_b.first) {
                    interPoint.second = min(in_point_value(last_a, cur_a, interPoint.first),
                                            in_point_value(last_b, cur_b, interPoint.first));
                    result.push_back(interPoint);
                }
            }
            result.push_back({cur_a.first, min(cur_a.second, cur_b.second)});
        }
        a.points.pop_front();
        last_a = cur_a;
        last_b = cur_b;
    }


    return result;
}

GeometricView CSWM_Slide(GeometricView input, int h) {
    if (h == 1) {
        return input;
    }
    // cerr << "-------------------" << endl;

    GeometricView result;
    pair<int, int> last_point = input.points.front();
    result.push_back(last_point);

    GeometricView listL;
    listL.push_back({last_point.first - h + 1, last_point.second});
    listL.push_back(last_point);

    for (int i = 1; i < input.points.size(); i++) {
        pair<int, int> next_point = input.points[i];
        bool line_above = true;

        for (int j = 1; j < listL.points.size(); j++) {
            pair previous_point = {listL.points[j - 1].first + h - 1, listL.points[j - 1].second},
                    current_point = {listL.points[j].first + h - 1, listL.points[j].second};
            if (previous_point.first >= next_point.first) {
                if (previous_point.first == next_point.first)
                    result.push_back(previous_point);
                break;
            }

            if (j != 1) result.push_back(previous_point);

            if (current_point.first > next_point.first) {
                current_point = {next_point.first, in_point_value(previous_point, current_point, next_point.first)};
            }
            if (current_point.second >= in_point_value(last_point, next_point, current_point.first)) {
                // cerr << "previous_point: " << previous_point.first << " " << previous_point.second << endl;
                // cerr << "current_point: " << current_point.first << " " << current_point.second << endl;
                // cerr << "next_point: " << next_point.first << " " << next_point.second << endl;
                // cerr << "last_point: " << last_point.first << " " << last_point.second << endl;
                auto interPoint = lines_intersection_by_points(previous_point, current_point, last_point, next_point);
                if (interPoint != pair<int, int>{INT_MAX, INT_MAX}) {
                    if (interPoint.first > result.points.back().first) {
                        // cerr << "interPoint1: " << interPoint.first << " " << interPoint.second << endl;
                        result.push_back(interPoint);
                    }
                    interPoint.first++;
                    if (interPoint.first < next_point.first && interPoint.first <= current_point.first) {
                        interPoint.second = min(in_point_value(previous_point, current_point, interPoint.first),
                                                in_point_value(last_point, next_point, interPoint.first));
                        // cerr << "interPoint2: " << interPoint.first << " " << interPoint.second << endl;
                        result.push_back(interPoint);
                    }
                }

                result.push_back(next_point);
                listL = GeometricView();
                listL.push_back({next_point.first - h + 1, next_point.second});
                listL.push_back(next_point);
                line_above = false;
                break;
            } else if (j + 1 == listL.points.size()) {
                //cerr << "current_point " << current_point.first << " " << current_point.second << endl;
                result.push_back(current_point);
            }
        }
        // cerr << "line above: " << line_above << endl;
        // cerr << "Result: ";
        // for (auto to: result.points) {
        //     cerr << to.first << " " << to.second << endl;
        // }

        if (line_above) {
            while (listL.points.size() > 1) {
                if (listL.points[listL.points.size() - 1].second <= next_point.second) {
                    break;
                }
                if (listL.points[listL.points.size() - 2].second >= next_point.second) {
                    listL.points.pop_back();
                } else {
                    auto q = lines_intersection_by_points(listL.points[listL.points.size() - 2],
                                                          listL.points[listL.points.size() - 1], next_point,
                                                          {next_point.first - 1, next_point.second});
                    // cerr << "point a: " << listL.points[listL.points.size() - 2].first << " " << listL.points[listL.points.size() - 2].second << endl;
                    // cerr << "point b: " << listL.points[listL.points.size() - 1].first << " " << listL.points[listL.points.size() - 1].second << endl;
                    // cerr << "point c: " << next_point.first << " " << next_point.second << endl;
                    // cerr << "point d: " << next_point.first - 1 << " " << next_point.second << endl;
                    // cerr << "q2 " << q.first << " " << q.second << endl;
                    listL.points.pop_back();
                    listL.points.push_back(q);
                    break;
                }
            }
            listL.push_back(next_point);

            while (listL.points.size() > 1) {
                if (listL.points[0].first >= next_point.first - h + 1) break;
                if (listL.points[0].first < next_point.first - h + 1 && listL.points[1].first <= next_point.first - h +
                    1) {
                    listL.points.pop_front();
                } else {
                    auto q = lines_intersection_by_points(listL.points[0], listL.points[1],
                                                          {next_point.first - h + 1, 10}, {
                                                              next_point.first - h + 1, 0
                                                          });
                    // cerr << "q " << q.first << " " << q.second << endl;
                    listL.points.pop_front();
                    // cerr << "listL: " << listL.points[0].first << " " << listL.points[0].second << endl;
                    listL.points.push_front(q);
                }
            }
        }
        // cerr << "listL: " << endl;
        // for (auto to: listL.points) {
        //     cerr << to.first << " " << to.second << endl;
        // }

        last_point = next_point;
    }
    //cerr << "--------------------------" << endl;

    for (auto to: listL.points) {
        if (to.first + h - 1 > last_point.first && to.first + h - 1 <= last_point.first + h - 1) {
            //cerr << "to: " << to.first + h - 1 << " " << to.second << endl;
            result.push_back({to.first + h - 1, to.second});
        }
    }
    return result;
}

#endif //GEOMETRIC_VIEW_COMMON
