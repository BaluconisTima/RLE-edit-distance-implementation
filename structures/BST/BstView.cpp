#ifndef BINARY_TREE_REPRESENTATION
#define BINARY_TREE_REPRESENTATION

#include <functional>
#include <stdio.h>
#include <bits/stl_pair.h>
#define endl '\n'
using namespace std;
#include "BstNode.cpp"
#include "../../common/utils.cpp"
#define float long double

namespace treap {
    void print_tree(node *v) {
        if (v->p != nullptr) {
            cerr << "Node: " << v << " Parent: " << v->p << '\n';
            exit(-1);
        }
        auto end_node = v->get_rightmost();
        bool bad = false;
        auto cur = v->get_leftmost();
        float last_x = cur->get_xl(), last_y = cur->get_yl();
        cerr << "------------------------\n";
        int tmin = INT_MAX;
        while (true) {
            cerr << fixed << setprecision(2) << cur->get_xl() << ' ' << cur->get_yl() << ' ' << cur->get_xr() << ' ' <<
                    cur->get_yr() << " | " << cur->type_l << ' ' << cur->getOurType() << ' ' << cur->type_r
                    << " | " << cur->our_time_cur << ' ' << cur->get_tmin() << ' ' << cur->dt << ' ' << cur->d_gradient <<
                    ' ' << cur->deltax << ' ' << cur->deltay << '\n';
            tmin = min(tmin, cur->get_tmin());
            if (cur->get_xl() > cur->get_xr()) {
                cerr << "Error: x_left > x_right" << '\n';
                bad = true;
            }
            if (abs(cur->get_xl() - last_x) > 0.000001) {
                cerr << "Error: get_xl !- last_x" << '\n';
                bad = true;
            }
            if (abs(cur->get_yl() - last_y) > 0.000001) {
                cerr << "Error: y_left != last_y" << '\n';
                bad = true;
            }
            last_x = cur->get_xr();
            last_y = cur->get_yr();
            if (cur == end_node) {
                break;
            }
            cur = cur->next();
        }
        if (tmin < v->get_tmin()) {
            cerr << "Error: tmin < root_tmin" << '\n';
            cerr << tmin << ' ' << v->get_tmin() << '\n';
            bad = true;
        }
        cerr << "------------------------\n";
        if (bad) {
            cerr << "Error: tree is not valid" << '\n';
            exit(-1);
        }
    }

    node *merge(node *v, node *u) {
        if (v == nullptr) {
            return u;
        }
        if (u == nullptr) {
            return v;
        }
        if (v->P > u->P) {
            v->push();
            v->r = merge(v->r, u);
            if (v->r != nullptr) {
                v->r->p = v;
            }
            v->pull();
            return v;
        } else {
            u->push();
            u->l = merge(v, u->l);
            if (u->l != nullptr) {
                u->l->p = u;
            }
            u->pull();
            return u;
        }
    }

    pair<node *, node *> split(node *v, float xm) {
        if (v == nullptr) {
            return {nullptr, nullptr};
        }

        v->push();
        if (xm < v->get_xl()) {
            pair<node *, node *> p = split(v->l, xm);
            if (p.first != nullptr) {
                p.first->p = nullptr;
            }
            v->l = p.second;
            if (v->l != nullptr) {
                v->l->p = v;
            }
            v->pull();
            return {p.first, v};
        }
        if (xm > v->get_xr()) {
            pair<node *, node *> p = split(v->r, xm);
            v->r = p.first;
            if (v->r != nullptr) {
                v->r->p = v;
            }
            if (p.second != nullptr) {
                p.second->p = nullptr;
            }
            v->pull();
            return {v, p.second};
        }

        if (abs(v->get_xl() - xm) < 0.0000001) {
            auto l = v->l;
            v->l = nullptr;
            if (l != nullptr) {
                l->p = nullptr;
            }
            v->pull();
            return {l, v};
        }
        if (abs(v->get_xr() - xm) < 0.0000001) {
            auto r = v->r;
            v->r = nullptr;
            if (r != nullptr) {
                r->p = nullptr;
            }
            v->pull();
            return {v, r};
        }
        float ym = value_in_m(v->get_xl(), v->get_yl(), v->get_xr(), v->get_yr(), xm);
        node *new_root = new node(xm, v->get_xr(), ym, v->get_yr());
        v->set_segment(v->get_xl(), xm, v->get_yl(), ym);
        auto r = v->r;

        v->r = nullptr;
        if (r != nullptr) {
            r->p = nullptr;
        }
        v->pull();

        if (abs(new_root->get_xl() - new_root->get_xr()) < 0.0000001) {
            return {v, r};
        }
        new_root = merge(new_root, r);
        return {v, new_root};
    }

    node *remove(node *v) {
        v->push();
        node *x = v->l;
        node *y = v->r;
        node *p = v->p;
        v->l = v->r = v->p = nullptr;
        v->push();
        v->pull();
        node *z = merge(x, y);
        if (p == nullptr) {
            if (z != nullptr) {
                z->p = nullptr;
            }
            return z;
        }
        if (p->l == v) {
            p->l = z;
        }
        if (p->r == v) {
            p->r = z;
        }
        while (true) {
            p->push();
            p->pull();
            if (p->p == nullptr) {
                break;
            }
            p = p->p;
        }
        return p;
    }

    node *smart_merge(node *v, node *u) {
        if (v == nullptr) {
            return u;
        }
        if (u == nullptr) {
            return v;
        }

        auto rv = v->get_rightmost(), lu = u->get_leftmost();
        if (rv->get_xr() != lu->get_xl() || rv->get_yr() != lu->get_yl()) {
            cerr << "Error: smart_merge: segments do not match" << endl;
            print_tree(v);
            cerr << endl;
            print_tree(u);
            exit(-1);
        }
        if (line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) == line_type(
                lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr())) {
            rv->set_segment(rv->get_xl(), lu->get_xr(), rv->get_yl(), lu->get_yr());
            u = remove(lu);
            lu = u->get_leftmost();
            if (lu != nullptr) {
                lu->update_types();
            }
            rv->update_types();

            if (u == nullptr) {
                return v;
            }
        } else
        // if rv is decreasing and lu is increasing, we need to add an artificial segment in the middle
            if (line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) == -1 &&
                line_type(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr()) == 1) {
                node *nf = (new node(rv->get_xr(), rv->get_xr(), rv->get_yr(), rv->get_yr()));
                v = merge(v, nf);
                rv->update_types();
                nf->update_types();
                rv = nf;
            }
        auto nr = merge(v, u);
        rv->update_types();
        lu->update_types();
        return nr;
    }


    pair<node *, node *> smart_split(node *v, float xm) {
        auto lv = v->get_leftmost();
        if (abs(lv->get_xl() - xm) < 0.00001) {
            auto boarder = new node(xm, xm, lv->get_yl(), lv->get_yl());
            lv->update_types();
            boarder->update_types();
            return {boarder, v};
        }

        auto rv = v->get_rightmost();
        if (abs(rv->get_xr() - xm) < 0.00001) {
            auto boarder = new node(xm, xm, rv->get_yr(), rv->get_yr());
            rv->update_types();
            boarder->update_types();
            return {v, boarder};
        }
        pair<node *, node *> p = split(v, xm);

        if (p.first != nullptr) {
            auto rf = p.first->get_rightmost();
            auto lf = p.first->get_leftmost();
            if (rf != lf && abs(rf->get_xr() - rf->get_xl()) < 0.00001) {
                p.first = remove(rf);
            }
        }

        if (p.second != nullptr) {
            auto ls = p.second->get_leftmost();
            auto rs = p.second->get_rightmost();
            if (ls != rs && abs(ls->get_xr() - ls->get_xl()) < 0.00001) {
                p.second = remove(ls);
            }
        }
        p.first->update_boarders();
        p.second->update_boarders();
        return p;
    }

    node *combine(node *v, node *u) {
        if (v == nullptr) {
            return u;
        }
        if (u == nullptr) {
            return v;
        }
        auto rv = v->get_rightmost(), ru = u->get_rightmost();
        auto lv = v->get_leftmost(), lu = u->get_leftmost();

        if (lv->get_yl() - lu->get_yl() < 0.000001) {
            return v;
        }
        if (rv->get_yr() - ru->get_yr() > 0.00001) {
            return u;
        }
        if (lv->get_xl() != lu->get_xl() || rv->get_xr() != rv->get_xr()) {
            exit(-1);
        }

        while (lv != nullptr) {
            while (lv->get_xr() - lu->get_xr() > 0.00001) {
                lu = lu->next();
            }
            float val = value_in_m(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr(), lv->get_xr());
            if (lv->get_yr() - val > 0.00001) {
                lv = lv->next();
                continue;
            } else break;
        }

        while (ru != nullptr) {
            while (rv->get_xl() - ru->get_xl() > 0.00001) {
                rv = rv->prev();
            }
            float val = value_in_m(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr(), ru->get_xl());
            if (!(abs(val - ru->get_yl()) > 0.00001 && val > ru->get_yl())) {
                ru = ru->prev();
                continue;
            } else break;
        }

        float xm = -INT_MIN;
        int l = max(lv->get_xl(), ru->get_xl()) * 2.0, r = min(lv->get_xr(), ru->get_xr()) * 2.0;
        while (l <= r) {
            int m = (l + r) / 2;
            float diff = value_in_m(ru->get_xl(), ru->get_yl(), ru->get_xr(), ru->get_yr(), m / 2.0) -
                         value_in_m(lv->get_xl(), lv->get_yl(), lv->get_xr(), lv->get_yr(), m / 2.0);

            if (diff >= 0.00001 || abs(diff) < 0.00001) {
                if (abs(diff) < 0.00001) {
                    xm = m / 2.0;
                    break;
                }
                r = m - 1;
            } else {
                l = m + 1;
            }
        }

        if (xm < 0) {
            cerr << "Error: combine: xm is -INT_MIN" << endl;
            cerr << "v " << endl;
            print_tree(v);

            cerr << "u " << endl;
            print_tree(u);
            cerr << endl;
            exit(-1);
        }

        pair<node *, node *> vsplit, usplit;
        if (abs(floor(xm) - xm) > 0.00001) {
            vsplit = smart_split(v, floor(xm) + 1);
            usplit = smart_split(u, floor(xm));
            node *nw = new node(floor(xm), floor(xm) + 1, usplit.first->get_rightmost()->get_yr(),
                                vsplit.second->get_leftmost()->get_yl());
            usplit.first = smart_merge(usplit.first, nw);
        } else {
            vsplit = smart_split(v, xm);
            usplit = smart_split(u, xm);
        }
        auto result = smart_merge(usplit.first, vsplit.second);
        auto left = result->get_leftmost();
        if (abs(left->get_xl() - left->get_xr()) < 0.00001) {
            result = remove(left);
        }
        auto right = result->get_rightmost();
        if (abs(right->get_xl() - right->get_xr()) < 0.00001) {
            result = remove(right);
        }
        result->update_boarders();
        return result;
    }

    void find_colapsed(node *v, vector<node *> &nodes, int time_to_collapse) {
        if (v == nullptr) {
            return;
        }
        v->updated_time_hash = 1;
        if (v->get_tmin() > time_to_collapse) {
            return;
        }
        v->push();

        if (v->our_time_cur == v->dt + time_to_collapse) {
            v->forDelete = true;
            nodes.push_back(v);
        }

        find_colapsed(v->l, nodes, time_to_collapse);
        find_colapsed(v->r, nodes, time_to_collapse);
        v->pull();
    }

    void update_all_toched(node *v) {
        if (v == nullptr) {
            return;
        }
        if (!v->updated_time_hash) return;
        v->push();
        v->updated_time_hash = 0;
        update_all_toched(v->l);
        update_all_toched(v->r);
        v->pull();
    }

    node *safeRemoveNode(node *root, node *node1) {
        if (node1 == nullptr) {
            cerr << "Error: safeRemoveNode called on nullptr" << endl;
            return root;
        }
        auto prv = node1->prev(),
                nxt = node1->next();
        if (node1->forDelete) {
            root = remove(node1);
        } else {
            cerr << "Error: safeRemoveNode called on node with tmin != 0" << endl;
            exit(-1);
        }

        if (prv != nullptr && nxt != nullptr && !nxt->forDelete && !prv->forDelete) {
            if (line_type(prv->get_xl(), prv->get_yl(), prv->get_xr(), prv->get_yr()) ==
                line_type(nxt->get_xl(), nxt->get_yl(), nxt->get_xr(), nxt->get_yr())) {
                prv->set_segment(prv->get_xl(), nxt->get_xr(), prv->get_yl(), nxt->get_yr());
                auto nxt_nxt = nxt->next();
                root = remove(nxt);
                nxt = nxt_nxt;
                if (nxt != nullptr) {
                    nxt->update_types();
                }
            } else
                if (line_type(prv->get_xl(), prv->get_yl(), prv->get_xr(), prv->get_yr()) == -1 &&
                    line_type(nxt->get_xl(), nxt->get_yl(), nxt->get_xr(), nxt->get_yr()) == 1) {
                    auto p = smart_split(root, prv->get_xr());
                    auto rv = p.first, lv = p.second;
                    auto nw = new node(prv->get_xr(), prv->get_xr(), prv->get_yr(), nxt->get_yr());
                    root = smart_merge(rv, nw);
                    root = smart_merge(root, lv);
                }
        }
        if (prv != nullptr) {
            prv->update_types();
        }
        if (nxt != nullptr) {
            nxt->update_types();
        }
        return root;
    }

    node *update_boarders(node *v) {
        auto left = v->get_leftmost();
        auto right = v->get_rightmost();

        if (line_type(left->get_xl(), left->get_yl(), left->get_xr(), left->get_yr()) == 1) {
            auto nw = new node(left->get_xl(), left->get_xl(), left->get_yl(), left->get_yl());
            v = smart_merge(nw, v);
        }

        if (line_type(right->get_xl(), right->get_yl(), right->get_xr(), right->get_yr()) == -1) {
            auto nw = new node(right->get_xr(), right->get_xr(), right->get_yr(), right->get_yr());
            v = smart_merge(v, nw);
        }
        v->update_boarders();
        return v;
    }

    node *SWM(node *v, int h) {
        h--;
        if (h == 0) return v;
        if (h < 0) {
            cerr << "Error: SWM called with h < 0" << endl;
            exit(-1);
        }
        cerr << "SWM called with h = " << h << endl;
        print_tree(v);

        while (h > 0) {
            v = update_boarders(v);
            cerr << "After update_boarders" << endl;
            print_tree(v);
            int x = min(h, v->get_tmin());
            if (x < 0) {
                cerr << "Error: SWM called with negative tmin" << endl;
                print_tree(v);
                exit(-1);
            }
            if (x == 0) {
                cerr << "No more time to remove" << endl;
                exit(-1);
            }
            h -= x;
            cerr << "Time to remove: " << x << endl;
            vector<node *> nodes;

            find_colapsed(v, nodes, x);
            v->add_t_root(x);
            update_all_toched(v);

            for (auto &node: nodes) {
                cerr << "Removing node: " << node << endl;
                v = safeRemoveNode(v, node);
                print_tree(v);
                v = update_boarders(v);
            }
        }
        auto left = v->get_leftmost();
        if (abs(left->get_xl() - left->get_xr()) < 0.00001) {
            v = safeRemoveNode(v, left);
        }
        auto right = v->get_rightmost();
        if (abs(right->get_xl() - right->get_xr()) < 0.00001) {
            v = safeRemoveNode(v, right);
        }
        print_tree(v);
        v->update_boarders();

        return v;
    }
} // namespace treap

using namespace treap;

#endif // BINARY_TREE_REPRESENTATION
