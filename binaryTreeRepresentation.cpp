#ifndef BINARY_TREE_REPRESENTATION
#define BINARY_TREE_REPRESENTATION

#include <functional>
#include <stdio.h>
#include <bits/stl_pair.h>
#define endl '\n'
using namespace std;
#include "BstNode.cpp"
#include "common_utils.cpp"

namespace treap {
    void print_tree(node *v) {
        return;
        // cerr << "root " << v << endl;
        // cerr << "root->l " << v->l << endl;
        // cerr << "root->r " << v->r << endl;
        if (v->p != nullptr) {
            cerr << "Node: " << v << " Parent: " << v->p << '\n';
            exit(-1);
        }
        auto end_node = v->get_rightmost();
        // cerr << "end_node: " << end_node << endl;
        bool bad = false;
        auto cur = v->get_leftmost();
        float last_x = cur->get_xl(), last_y = cur->get_yl();
        cerr << "------------------------\n";
        int tmin = INT_MAX;
        while (true) {
            cerr << fixed << setprecision(2) << cur->get_xl() << ' ' << cur->get_yl() << ' ' << cur->get_xr() << ' ' <<
                    cur->get_yr() << " | " << cur->type_l << ' ' << cur->our_type << ' ' << cur->type_r
                    << " | " << cur->our_time() << ' ' << cur->get_tmin() << ' ' << cur->dt << ' ' << cur->d_gradient <<
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
            //cerr << cur->l << ' ' << cur->r << endl;
            if (cur == end_node) {
                break;
            }
            cur = cur->next();
        }
        // if (tmin < v->get_tmin()) {
        //     cerr << "Error: tmin < root_tmin" << '\n';
        //     cerr << tmin << ' ' << v->get_tmin() << '\n';
        //     bad = true;
        // }
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
            // cerr << "V right" << endl;
            v->push();
            v->r = merge(v->r, u);
            if (v->r != nullptr) {
                v->r->p = v;
            }
            v->pull();
            return v;
        } else {
            // cerr << "U left" << endl;
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
        //cerr << "NODE " << v << ' ' << v->get_xl() << ' ' << v->get_xr() << ' ' << xm << endl;
        //cerr << "NODE " << v << ' ' << v->get_yl() << ' ' << v->get_yr() << ' ' << xm << endl;

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
        // cerr << v->get_xl() << ' ' << v->get_yl() << ' ' << v->get_xr() << ' ' << v->get_yr() << endl;
        // cerr << "xm ym: " << xm << ' ' << ym << endl;
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
        // returns the new root
        v->push();
        node *x = v->l;
        node *y = v->r;
        node *p = v->p;
        v->l = v->r = v->p = nullptr;
        v->push();
        v->pull(); // now v might be reusable...
        node *z = merge(x, y);
        // cerr << "z: " << z << endl;
        // cerr << "x: " << x << endl;
        // cerr << "y: " << y << endl;
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
        //cerr << "smart_merge x: " << rv->get_xr() << ' ' << lu->get_xl() << endl;
        //cerr << "smart_merge y: " << rv->get_yr() << ' ' << lu->get_yl() << endl;
        if (rv->get_xr() != lu->get_xl() || rv->get_yr() != lu->get_yl()) {
            cerr << "Error: smart_merge: segments do not match" << endl;
            exit(-1);
        }
        // cerr << "u1 " << endl;
        // print_tree(u);
        // cerr << "v1 " << endl;
        // print_tree(v);
        //cerr << line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) << ' ' <<  line_type(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr()) << endl;
        if (line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) == line_type(
                lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr())) {
            rv->set_segment(rv->get_xl(), lu->get_xr(), rv->get_yl(), lu->get_yr());
            //cerr << "smart_merge: segments are the same type, merging" << endl;
            u = remove(lu);
            // cerr << "smart_merge: removed lu: " << lu << endl;
            //cerr << "smart_merge: removed lu: " << lu << endl;
            lu = u->get_leftmost();
            // cerr << "smart_merge: lu after remove: " << lu << endl;
            if (lu != nullptr) {
                lu->update_types();
                //lu->update_up();
            }
            if (rv != nullptr) {
                rv->update_types();
                //rv->update_up();
            }

            if (u == nullptr) {
                return v;
            }
        } else
        // if rv is decreasing and lu is increasing, we need to add an artificial segment in the middle
        // cerr << line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) << ' ' << line_type(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr()) << endl;
            if (line_type(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr()) == -1 &&
                line_type(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr()) == 1) {
                node *nf = (new node(rv->get_xr(), rv->get_xr(), rv->get_yr(), rv->get_yr()));
                // cerr << "smart_merge: segments are different types, adding artificial segment" << endl;
                v = merge(v, nf);
                // cerr << "smart_merge: merged with artificial segment: " << nf << endl;
                rv->update_types();
                // cerr << "smart_merge: rv after merge: " << rv << endl;
                nf->update_types();
                // cerr << "smart_merge: nf after merge: " << nf << endl;
                rv = v->get_rightmost();
            }
        //cerr << "smart_merge: rv after all: " << rv << endl;
        if (lu != nullptr) {
            lu->update_types();
            //lu->update_up();
        }
        if (rv != nullptr) {
            rv->update_types();
            //rv->update_up();
        }
        //cerr << v << ' ' << u << endl;
        //print_tree(u);
        // cerr << endl;
        // print_tree(v);
        // cerr << endl;
        auto nr = merge(v, u);
        //print_tree(nr);
        //cerr << endl;
        rv->update_types();
        //rv->update_up();
        lu->update_types();
        //lu->update_up();
        return nr;
    }


    pair<node *, node *> smart_split(node *v, float xm) {
        auto lv = v->get_leftmost();
        if (abs(lv->get_xl() - xm) < 0.00001) {
            auto boarder = new node(xm, xm, lv->get_yl(), lv->get_yl());
            lv->update_up();
            return {boarder, v};
        }

        auto rv = v->get_rightmost();
        if (abs(rv->get_xr() - xm) < 0.00001) {
            auto boarder = new node(xm, xm, rv->get_yr(), rv->get_yr());
            rv->update_up();
            return {v, boarder};
        }
        pair<node *, node *> p = split(v, xm);

        // cerr << "smart_split: " << p.first << ' ' << p.second << endl;
        if (p.first != nullptr) {
            auto rf = p.first->get_rightmost();
            auto lf = p.first->get_leftmost();
            // cerr << "smart_split: rightmost first: " << rf->get_xl() << ' ' << rf->get_yr() << endl;
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
        p.second->get_leftmost()->update_types();
        //p.second->get_leftmost()->update_up();
        p.first->get_rightmost()->update_types();
        //p.first->get_rightmost()->update_up();
        return p;
    }

    node *combine(node *v, node *u) {
        //cerr << "COMBINE CALLED" << v << ' ' << u << endl;
        if (v == nullptr) {
            return u;
        }
        if (u == nullptr) {
            return v;
        }
        // cerr << "v " << endl;
        // print_tree(v);
        //
        // cerr << "u " << endl;
        // print_tree(u);
        // cerr << endl;
        auto rv = v->get_rightmost(), ru = u->get_rightmost();

        auto lv = v->get_leftmost(), lu = u->get_leftmost();
        //cerr << lv << ' ' << lu << endl;
        //cerr << lv->get_yl() <<  ' '<<  lu->get_yl() << endl;
        if (lv->get_yl() - lu->get_yl() < 0.000001) {
            return v;
        }
        if (rv->get_yr() - ru->get_yr() > 0.00001) {
            return u;
        }
        if (lv->get_xl() != lu->get_xl() || rv->get_xr() != rv->get_xr()) {
            // cerr << "Error: combine: segments do not match" << endl;
            exit(-1);
        }
        //cerr << "HERE IS SAFE" << endl;
        // find correct lv
        while (lv != nullptr) {
            //cerr << "lv: " << lv->get_xl() << ' ' << lv->get_yl() << ' ' << lv->get_xr() << ' ' << lv->get_yr() << endl;
            //cerr << "lu: " << lu->get_xl() << ' ' << lu->get_yl() << ' ' << lu->get_xr() << ' ' << lu->get_yr() << endl;
            while (lv->get_xr() - lu->get_xr() > 0.00001) {
                lu = lu->next();
                //cerr << "lu: " << lu << ' ' << lu->get_xl() << ' ' << lu->get_yl() << ' ' << lu->get_xr() << ' ' << lu->get_yr() << endl;
            }
            //cerr << "lu after: " << lu->get_xl() << ' ' << lu->get_yl() << ' ' << lu->get_xr() << ' ' << lu->get_yr() << endl;
            float val = value_in_m(lu->get_xl(), lu->get_yl(), lu->get_xr(), lu->get_yr(), lv->get_xr());
            //cerr << val << endl;
            if (lv->get_yr() - val > 0.00001) {
                lv = lv->next();
                continue;
            } else break;
        }
        // cerr << "lv after: " << lv->get_xl() << ' ' << lv->get_yl() << ' ' << lv->get_xr() << ' ' << lv->get_yr() << endl;
        // cerr << "lu after: " << lu->get_xl() << ' ' << lu->get_yl() << ' ' << lu->get_xr() << ' ' << lu->get_yr() << endl;


        // find correct ru
        while (ru != nullptr) {
            //cerr << "ru: " << ru->get_xl() << ' ' << ru->get_yl() << ' ' << ru->get_xr() << ' ' << ru->get_yr() << endl;
            while (rv->get_xl() - ru->get_xl() > 0.00001) {
                //cerr << "rv: " << rv << ' ' << rv->get_xl() << ' ' << rv->get_yl() << ' ' << rv->get_xr() << ' ' << rv->get_yr() << endl;
                rv = rv->prev();
            }
            //cerr << "rv after: " << rv->get_xl() << ' ' << rv->get_yl() << ' ' << rv->get_xr() << ' ' << rv->get_yr() << endl;
            float val = value_in_m(rv->get_xl(), rv->get_yl(), rv->get_xr(), rv->get_yr(), ru->get_xl());
            if (!(abs(val - ru->get_yl()) > 0.00001 && val > ru->get_yl())) {
                ru = ru->prev();
                continue;
            } else break;
        }
        float xm = -INT_MIN;

        // cerr << lv->get_xl() << ' ' << lv->get_yl() << ' ' << lv->get_xr() << ' ' << lv->get_yr() << endl;
        // cerr << ru->get_xl() << ' ' << ru->get_yl() << ' ' << ru->get_xr() << ' ' << ru->get_yr() << endl;
        // find x_m
        int l = max(lv->get_xl(), ru->get_xl()) * 2.0, r = min(lv->get_xr(), ru->get_xr()) * 2.0;
        while (l <= r) {
            int m = (l + r) / 2;
            float diff = value_in_m(ru->get_xl(), ru->get_yl(), ru->get_xr(), ru->get_yr(), m / 2.0) -
                         value_in_m(lv->get_xl(), lv->get_yl(), lv->get_xr(), lv->get_yr(), m / 2.0);
            //cerr << "m: " << m/2.0 << ' ' << diff << endl;

            if (diff >= 0.00001 || abs(diff) < 0.00001) {
                if (abs(diff) < 0.00001) {
                    xm = m / 2.0;
                    //break;
                }
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        // cerr << "xm: " << fixed << setprecision(5) << xm << endl;

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
            // cerr << "Splitting with float " << floor(xm) << endl;
            vsplit = smart_split(v, floor(xm) + 1);
            //cerr << "V split: " << vsplit.first << ' ' << vsplit.second << endl;
            usplit = smart_split(u, floor(xm));
            // cerr << "U split: " << usplit.first << ' ' << usplit.second << endl;
            node *nw = new node(floor(xm), floor(xm) + 1, usplit.first->get_rightmost()->get_yr(),
                                vsplit.second->get_leftmost()->get_yl());
            // cerr << "New node: " << nw->get_xl() << ' ' << nw->get_yl() << ' ' << nw->get_xr() << ' ' << nw->get_yr() << endl;

            usplit.first = smart_merge(usplit.first, nw);
        } else {
            vsplit = smart_split(v, xm);
            usplit = smart_split(u, xm);
        }


        // cerr << "U split: " << endl;
        // print_tree(usplit.first);
        // cerr << "V split: " << endl;
        // print_tree(vsplit.second);
        auto result = smart_merge(usplit.first, vsplit.second);
        auto left = result->get_leftmost();
        if (abs(left->get_xl() - left->get_xr()) < 0.00001) {
            result = remove(left);
        }
        result->get_leftmost()->update_up();
        auto right = result->get_rightmost();
        if (abs(right->get_xl() - right->get_xr()) < 0.00001) {
            result = remove(right);
        }
        result->get_rightmost()->update_up();
        // cerr << "Result after merge: " << endl;
        //   print_tree(result);
        return result;
    }

    void find_colapsed(node *v, vector<node *> &nodes, int time_to_collapse) {
        if (v == nullptr) {
            return;
        }
        v->updated_time_hash = 1;
        // cerr << "in node " << v << ' ' << v->get_tmin() << ' ' << v->dt  << ' ' << v->l << ' ' << v->r << endl;
        if (v->get_tmin() != time_to_collapse) {
            // cerr  << ":( " << v->get_tmin() << endl;
            return;
        }

        // cerr << "Visiting node: " << v << ' ' << v->get_xl() << ' ' << v->get_yl() << ' ' << v->get_xr() << ' ' << v->get_yr() << endl;
        // cerr << v->our_time() << ' ' << v->dt << endl;
        v->push();

        if (v->our_time() == v->dt + time_to_collapse) {
            // cerr << "Found colapsed node: " << v << endl;
            // cerr << "Node: " << v->get_xl() << ' ' << v->get_yl() << ' ' << v->get_xr() << ' ' << v->get_yr() << endl;
            // cerr << "Node: " << v->type_l << ' ' << v->our_type << ' ' << v->type_r << endl;
            // cerr << v->our_time() << ' ' << v->dt << endl;
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
        v->push();
        if (!v->updated_time_hash) return;
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
            // cerr << node1->get_xl() << ' ' << node1->get_yl() << ' ' << node1->get_xr() << ' ' << node1->get_yr() << endl;
            // cerr << "safeRemoveNode called on node with zero length" << endl;
            root = remove(node1);
            //cerr << "Removed node: " << node1 << ' ' << root << endl;
            // print_tree(root);
        } else {
            // cerr << "Error: safeRemoveNode called on node with tmin != 0" << endl;
            //return root;
            cerr << "Error: safeRemoveNode called on node with tmin != 0" << endl;
            // cerr << node1->get_xl() << ' ' << node1->get_yl() << ' ' << node1->get_xr() << ' ' << node1->get_yr() << endl;
            // cerr << "Node: " << node1 << " tmin: " << node1->dt << ' ' << node1->our_time() << endl;
            exit(-1);
        }
        if (prv != nullptr) {
            prv->update_types();
            //prv->update_up();
        }
        if (nxt != nullptr) {
            nxt->update_types();
            //nxt->update_up();
        }
        if (prv != nullptr && nxt != nullptr && !nxt->forDelete && !prv->forDelete) {
            if (line_type(prv->get_xl(), prv->get_yl(), prv->get_xr(), prv->get_yr()) ==
                line_type(nxt->get_xl(), nxt->get_yl(), nxt->get_xr(), nxt->get_yr())) {
                //cerr << "Merging segments: " << prv << ' ' << nxt << endl;
                auto pre_nxt = nxt->next();
                root = remove(nxt);
                prv->set_segment(prv->get_xl(), nxt->get_xr(), prv->get_yl(), nxt->get_yr());
                prv->update_types();
                //prv->update_up();
                if (pre_nxt != nullptr) {
                    pre_nxt->update_types();
                    //pre_nxt->update_up();
                }
            } else
            // extra segment needed
                if (line_type(prv->get_xl(), prv->get_yl(), prv->get_xr(), prv->get_yr()) == -1 &&
                    line_type(nxt->get_xl(), nxt->get_yl(), nxt->get_xr(), nxt->get_yr()) == 1) {
                    //cerr << "Adding artificial segment between: " << prv << ' ' << nxt << endl;
                    auto p = smart_split(nxt, prv->get_xr());
                    auto rv = p.first, lv = p.second;
                    auto nw = new node(prv->get_xr(), prv->get_xr(), prv->get_yr(), nxt->get_yr());
                    root = smart_merge(rv, nw);
                    root = smart_merge(root, lv);
                }
        }
        //cerr << "Removed node: " << node1 << ' ' << root << endl;
        return root;
    }

    node *update_boarders(node *v) {
        auto left = v->get_leftmost();
        // if (abs(left->get_xl() - left->get_xr()) < 0.00001) {
        //   v = remove(left);
        //   left = v->get_leftmost();
        //   left->update_types();
        //   left->update_up();
        // }
        auto right = v->get_rightmost();
        // if (abs(right->get_xl() - right->get_xr()) < 0.00001) {
        //   v = remove(right);
        //   right = v->get_rightmost();
        //   right->update_types();
        //   right->update_up();
        // }

        // cerr << left << endl;
        //cerr << "leftmost line type " << left->get_xl() << ' ' << left->get_yl() << ' ' << left->get_xr() << ' ' << left->get_yr() << endl;
        // cerr << "leftmost: " << left->get_xl() << ' ' << left->get_yl() << ' ' << left->get_xr() << ' ' << left->get_yr() << endl;
        if (line_type(left->get_xl(), left->get_yl(), left->get_xr(), left->get_yr()) == 1) {
            auto nw = new node(left->get_xl(), left->get_xl(), left->get_yl(), left->get_yl());
            v = smart_merge(nw, v);
            left->update_types();
            //left->update_up();
            nw->update_types();
            //nw->update_up();
        }
        // cerr << "rightmost line type " << line_type(right->get_xl(), right->get_yl(), right->get_xr(), right->get_yr()) << endl;
        // cerr << "rightmost: " << right->get_xl() << ' ' << right->get_yl() << ' ' << right->get_xr() << ' ' << right->get_yr() << endl;
        if (line_type(right->get_xl(), right->get_yl(), right->get_xr(), right->get_yr()) == -1) {
            // cerr << "Adding new node to the right" << endl;
            auto nw = new node(right->get_xr(), right->get_xr(), right->get_yr(), right->get_yr());
            // cerr << "New node: " << nw->get_xl() << ' ' << nw->get_yl() << ' ' << nw->get_xr() << ' ' << nw->get_yr() << endl;
            v = smart_merge(v, nw);
            right->update_types();
            nw->update_types();
            //right->update_up();
            //nw->update_up();
            //print_tree(v);
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

        //cerr << "SWM called with h = " << h << endl;
        //cerr << "Initial tree: " <<  h << endl;
        print_tree(v);
        // check borders and get rid of -I and D-
        while (h > 0) {
            v = update_boarders(v);
            //cerr << "V parrent " << v->p << endl;

            //cerr << "with updated boarders:" << endl;
            print_tree(v);
            int x = min(h, v->get_tmin());
            // cerr << "x = " << x << endl;

            if (x < 0) {
                cerr << "Error: SWM called with negative tmin" << endl;
                exit(-1);
            }
            //cerr << "x = " << x << endl;
            if (x == 0) {
                cerr << "No more time to remove" << endl;
                exit(-1);
            }
            // cerr << "t_min = " << v->get_tmin() << endl;
            h -= x;
            // cerr << "new t_min = " << v->get_tmin() << endl;
            vector<node *> nodes;
            find_colapsed(v, nodes, x);
            v->add_t_root(x);
            //cerr << "added t" << endl;
            print_tree(v);
            update_all_toched(v);
            //cerr << "updated toched" << endl;
            print_tree(v);

            for (auto &node: nodes) {
                // cerr << "Removing node: " << node << ' ' << node->our_time() << ' '  << node->dt << endl;
                v = safeRemoveNode(v, node);
                v = update_boarders(v);
                // cerr << v << endl;
            }
            // cerr << "Removed " << nodes.size() << " nodes" << endl;
            // print_tree(v);
        }
        //cerr << "Updated tree:" << endl;
        //print_tree(v);
        //delete boarders if they are one point segments
        auto left = v->get_leftmost();
        if (abs(left->get_xl() - left->get_xr()) < 0.00001) {
            // cerr << "Removing leftmost node: " << left << endl;
            v = remove(left);
        }
        auto right = v->get_rightmost();
        if (abs(right->get_xl() - right->get_xr()) < 0.00001) {
            // cerr << "Removing rightmost node: " << right << endl;
            v = remove(right);
        }

        left = v->get_leftmost();
        left->update_types();
        //left->update_up();
        right = v->get_rightmost();
        right->update_types();
        //right->update_up();
        // cerr << "Final tree:" << endl;
        print_tree(v);
        // cerr << endl;
        return v;
    }

    // int get_size(node* v) {
    //   return (v != nullptr ? v->sz : 0);
    // }

    template<typename... T>
    void apply(node *v, T... args) {
        //v->unsafe_apply(args...);
    }

    // void reverse(node* v) {
    //   v->unsafe_reverse();
    // }
} // namespace treap

using namespace treap;

#endif // BINARY_TREE_REPRESENTATION
