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

int types_updated_counter = 0;

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
            v->type_r = nxt->getOurType();
        } else {
            if (nxt == nullptr || nxt == v)
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
            v->type_l = prv->getOurType();
        } else {
            if (prv == nullptr || prv == v)
                v->type_l = INT_MAX;
        }
        v->pull();
    }

    int getOurType() {
        push_t();
        int new_our_type = line_type(get_xl(), get_yl(), get_xr(), get_yr());
        pull();
        return new_our_type;
    }

    void update_types() {
        types_updated_counter++;
        if (forDelete) {
            return;
        }
        push();
        our_type = getOurType();
        update_left();
        update_right();
        update_our_time();
        update_extra_shift_left();
        update_extra_shift_right();
        pull();
        update_up();
    }

    node *l;
    node *r;
    node *p;
    // declare extra variables:
    int P;
    int type_l, type_r, our_type, dt;
    float deltax, deltay;
    float d_gradient = 0;
    bool forDelete = false;
    int updated_time_hash = 0;
    int sz = 1;
    int our_time_cur = INT_MAX;

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
        pull();
        update_types();
        auto nxt = next();
        if (nxt != nullptr && !nxt->forDelete && nxt != this) {
           nxt->push();
           nxt->pull();
           nxt->update_types();
        }
        auto prv = prev();
        if (prv != nullptr && !prv->forDelete && prv != this) {
            prv->push();
            prv->pull();
            prv->update_types();
        }
    }

    int dist() {
        return abs(get_xl_without_shift() - get_xr_without_shift())
               + abs(get_yl_without_shift() - get_yr_without_shift());
    }

    void update_our_time() {
        if ((type_l == 1 && our_type == -1 && type_r == 0) ||
            (type_l == 1 && our_type == 0 && type_r == -1) ||
            (type_l == 0 && our_type == 1 && type_r == -1)) {
            our_time_cur = dist();
            return;
        }
        our_time_cur = INT_MAX;
    }

    pair<float, float> extra_shift_left = {0, 0},
            extra_shift_right = {0, 0};

    void update_extra_shift_left() {
        if (type_l < -5 || type_l > 5) {
           extra_shift_left = {0, 0};
            return;
        }
        extra_shift_left = {
            shifts[type_l + 1][our_type + 1].first,
            shifts[type_l + 1][our_type + 1].second
        };
    }

    void update_extra_shift_right() {
        if (type_r < -5 || type_r > 5) {
            extra_shift_right = {1, 0};
            return;
        }
        extra_shift_right = {
            shifts[our_type + 1][type_r + 1].first,
            shifts[our_type + 1][type_r + 1].second
        };
    }

    int get_tmin() {
        return min(tmin, our_time_cur) - dt;
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
        auto cur = this;
        while (cur != nullptr) {
            cur->pull();
            cur = cur->p;
        }
    }

    void update_boarders() {
        update_types();
        auto lm = get_leftmost(),
                rm = get_rightmost();
        lm->push();
        rm->push();
        lm->update_types();
        rm->update_types();
    }

    void update_2_boarders() {
        update_types();
        auto lm = get_leftmost(),
                rm = get_rightmost();
        lm->push();
        rm->push();
        lm->update_types();
        rm->update_types();

        lm = lm->next();
        if (lm != nullptr) {
            lm->push();
            lm->update_types();
        }

        rm = rm->prev();
        if (rm != nullptr) {
            rm->push();
            rm->update_types();
        }
    }

private:
    void add_t(int t) {
        if (d_gradient == 0) {
            dt += t;
        } else {
            if (type_l == 1 || type_r == 1 || our_type == 1) {
                deltax += t;
            }
        }
    }

    void add_delta(float dx, float dy) {
        deltax += dx;
        deltay += dy;
    }

    void move_gradient(float d) {
        if (d == 0) return;
        push_t();
        if (abs(our_type) < 2) our_type += d;
        if (abs(type_l) < 2) type_l += d;
        if (abs(type_r) < 2) type_r += d;
        update_extra_shift_left();
        update_extra_shift_right();

        deltay += d * deltax;
        d_gradient += d;
        pull();
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
        pull();
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
        return xl + deltax + extra_shift_left.first * dt;
    }

    float get_xr() {
        return xr + deltax + extra_shift_right.first * dt;
    }

    float get_yl() {
        auto shift = extra_shift_left;
        return yl + deltay + d_gradient * (xl + shift.first * dt) + shift.second * dt;
    }

    float get_yr() {
        auto shift = extra_shift_right;
        return yr + deltay + d_gradient * (xr + shift.first * dt) + shift.second * dt;
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
        pair<float, float> shift_left = extra_shift_left;
        pair<float, float> shift_right = extra_shift_right;
        xl += shift_left.first * dt;
        xr += shift_right.first * dt;
        yl += shift_left.second * dt;
        yr += shift_right.second * dt;
        our_time_cur -= dt;
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
        our_time_cur -= dt;
        dt = 0;
        deltax = 0;
        deltay = 0;
        d_gradient = 0;
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
