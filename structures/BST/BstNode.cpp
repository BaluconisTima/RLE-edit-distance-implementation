#ifndef BST_NODE
#define BST_NODE
using namespace std;
#include <string>
#include <random>
#include "../../common/utils.cpp"

vector<vector<pair<float, float> > > shifts = {
    {{10000000000.0, 10000000000.0}, {0, 0}, {20000000000.0, 20000000000.0}},
    {{0, 0}, {30000000000.0, 30000000000.0}, {1, 0}},
    {{0.5, -0.5}, {1, 0}, {40000000000.0, 40000000000.0}}
};

class node {
public:
    node *next() {
        auto v = this;
        if (v->r == nullptr) {
            while (v->p != nullptr && v->p->r == v) {
                v = v->p;
            }
            return v->p;
        }
        v->push();
        v = v->r;
        while (v->l != nullptr) {
            v->push();
            v = v->l;
        }
        return v;
    }

    node *prev() {
        auto v = this;
        if (v->l == nullptr) {
            while (v->p != nullptr && v->p->l == v) {
                v = v->p;
            }
            return v->p;
        }
        v->push();
        v = v->l;
        while (v->r != nullptr) {
            v->push();
            v = v->r;
        }
        return v;
    }


    void update_right(node *nxt = nullptr) {
        auto v = this;
        v->push();
        if (nxt == nullptr) {
            nxt = v->next();
        }
        if (nxt != nullptr && nxt != v && !nxt->forDelete) {
            nxt->updateOurType();
            nxt->type_l = our_type;
            v->type_r = nxt->our_type;
            nxt->update_up();
        } else {
            v->type_r = INT_MAX;
        }
        v->pull();
    }

    void update_left(node *prv = nullptr) {
        auto v = this;
        v->push();
        if (prv == nullptr) {
            prv = v->prev();
        }
        if (prv != nullptr && prv != v && !prv->forDelete) {
            prv->updateOurType();
            prv->type_r = our_type;
            v->type_l = prv->our_type;
            prv->update_up();
        } else {
            v->type_l = INT_MAX;
        }
        v->pull();
    }

    void updateOurType() {
        push_t();
        our_type = line_type(get_xl(), get_yl(), get_xr(), get_yr());
        pull();
    }

    void update_types() {
        if (forDelete) {
            return;
        }
        updateOurType();
        update_left();
        update_right();
        update_up();
    }

    node *l;
    node *r;
    node *p;
    // declare extra variables:
    int P;
    int type_l, type_r, dt;
    int our_type = INT_MAX;
    float deltax, deltay;
    float d_gradient = 0;
    bool forDelete = false;
    int updated_time_hash = 0;
    int sz = 1;

private:
    float xl, xr, yl, yr;
    int tmin;

public:
    node(float _xl = 0, float _xr = 0, float _yl = 0, float _yr = 0): type_l(INT_MAX), type_r(INT_MAX), dt(0) {
        l = r = p = nullptr;
        // init extra variables:
        P = rand();
        xl = _xl;
        xr = _xr;
        yl = _yl;
        yr = _yr;
        sz = 1;
        our_type = line_type(_xl, _xr, _yl, _yr);
        deltax = 0;
        deltay = 0;
        tmin = INT_MAX;
        forDelete = false;
    }

    void set_segment(float _xl = 0, float _xr = 0, float _yl = 0, float _yr = 0) {
        push();
        xl = _xl;
        xr = _xr;
        yl = _yl;
        yr = _yr;
        update_types();
        pull();
        //update_types();
    }

    int dist() {
        return abs(get_xl_without_shift() - get_xr_without_shift())
               + abs(get_yl_without_shift() - get_yr_without_shift());
    }

    int our_time() {
        if ((type_l == 1 && our_type == -1 && type_r == 0) ||
            (type_l == 1 && our_type == 0 && type_r == -1) ||
            (type_l == 0 && our_type == 1 && type_r == -1)) {
            return dist();
        }
        return INT_MAX;
    }

    pair<float, float> get_extra_shift_left() {
        if (type_l < -5 || type_l > 5) {
            return {0, 0};
        }
        return {
            shifts[type_l + 1][our_type + 1].first * dt,
            shifts[type_l + 1][our_type + 1].second * dt
        };
    }

    pair<float, float> get_extra_shift_right() {
        if (type_r < -5 || type_r > 5) {
            return {dt, 0};
        }
        return {
            shifts[our_type + 1][type_r + 1].first * dt,
            shifts[our_type + 1][type_r + 1].second * dt
        };
    }

    int get_tmin() {
        return min(tmin, our_time()) - dt;
    }

    node *get_leftmost(int d = 0) {
        auto v = this;
        if (v == nullptr) {
            return nullptr;
        }
        v->push();
        auto cur = v;
        if (v->l != nullptr) {
            cur = v->l->get_leftmost(d + 1);
        }
        v->pull();
        return cur;
    }

    node *get_rightmost() {
        auto v = this;
        if (v == nullptr) {
            return nullptr;
        }
        v->push();
        auto cur = v;
        if (v->r != nullptr) {
            cur = v->r->get_rightmost();
        }
        v->pull();
        return cur;
    }

    void update_up() {
        if (this == nullptr) {
            cerr << "null func" << endl;
            exit(-1);
        }
        auto cur = this;
        while (cur != nullptr) {
            cur->pull();
            cur = cur->p;
        }
    }

    void update_boarders() {
        auto lm = get_leftmost(),
                rm = get_rightmost();
        lm->push();
        rm->push();
        lm->update_types();
        rm->update_types();
        lm->update_up();
        rm->update_up();
    }

private:
    void add_t(int t) {
        if (d_gradient == 0) {
            dt += t;
        } else {
            if (type_l == 1 || type_r == 1) {
                deltax += t;
            }
        }
    }

    void add_delta(float dx, float dy) {
        deltax += dx;
        deltay += dy;
    }

    void move_gradient(float d) {
        if (abs(d) < 0.00001) {
            return;
        }
        deltay += d * deltax;
        d_gradient += d;
    }

public:
    void add_t_root(int t) {
        update_boarders();
        if (this->p != nullptr) {
            cerr << "Error: add_t_root called on non-root node" << endl;
            exit(-1);
        }
        add_t(t);
        update_boarders();
    }

    void add_delta_root(float dx, float dy) {
        update_boarders();
        if (this->p != nullptr) {
            cerr << "Error: add_delta_root called on non-root node" << endl;
            exit(-1);
        }
        add_delta(dx, dy);
        update_boarders();
    }

    void move_gradient_root(float d) {
        update_boarders();
        if (this->p != nullptr) {
            cerr << "Error: move_gradient_root called on non-root node" << endl;
            exit(-1);
        }
        move_gradient(d);
        update_boarders();
    }

    float get_xl_without_shift() {
        return xl + deltax;
    }

    float get_xr_without_shift() {
        return xr + deltax;
    }

    float get_yl_without_shift() {
        return yl + deltay + d_gradient * xl;
    }

    float get_yr_without_shift() {
        return yr + deltay + d_gradient * xr;
    }

    float get_xl() {
        return xl + deltax + get_extra_shift_left().first;
    }

    float get_xr() {
        return xr + deltax + get_extra_shift_right().first;
    }

    float get_yl() {
        auto shift = get_extra_shift_left();
        return yl + deltay + d_gradient * (xl + shift.first) + shift.second;
    }

    float get_yr() {
        auto shift = get_extra_shift_right();
        return yr + deltay + d_gradient * (xr + shift.first) + shift.second;
    }

    void push_stuff() {
        add_t(p->dt);
        move_gradient(p->d_gradient);
        add_delta(p->deltax, p->deltay);
    }

    void push_t() {
        if (dt == 0) return;
        if (l != nullptr) {
            l->add_t(dt);
        }
        if (r != nullptr) {
            r->add_t(dt);
        }
        pair<float, float> shift_left = get_extra_shift_left();
        pair<float, float> shift_right = get_extra_shift_right();
        xl += shift_left.first;
        xr += shift_right.first;
        yl += shift_left.second;
        yr += shift_right.second;
        dt = 0;
    }

    void push() {
        if (l != nullptr) {
            l->push_stuff();
        }
        if (r != nullptr) {
            r->push_stuff();
        }
        yl = get_yl();
        yr = get_yr();
        xl = get_xl();
        xr = get_xr();
        dt = 0;
        deltax = 0;
        deltay = 0;
        bool need_update_types = (abs(d_gradient) > 0.001);
        d_gradient = 0;
        if (need_update_types) {
            if (l != nullptr) {
                l->update_types();
            }
            if (r != nullptr) {
                r->update_types();
            }
        }
        pull();
    }

    void pull() {
        sz = 1;
        tmin = INT_MAX;
        if (l != nullptr) {
            l->p = this;
            tmin = min(tmin, l->get_tmin());
            sz += l->sz;
        }
        if (r != nullptr) {
            r->p = this;
            tmin = min(tmin, r->get_tmin());
            sz += r->sz;
        }
    }
};

#endif // BST_NODE`
