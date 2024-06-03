#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include "sphere.h"
#include "vector.h"
#include "raio.h"
#include <vector>

using namespace std;

class sphere_list{
    public:
        sphere_list() {}
        sphere_list(sphere l) {add(l);}

        void clear() {list.clear();}
        void add(sphere l) {list.push_back(l);}

        bool hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;
    public:
        vector<sphere> list;
};

bool sphere_list::hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& l : list) {
        if (l.hit(raio, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif
