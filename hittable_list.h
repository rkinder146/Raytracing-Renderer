#pragma once

#include "hittable.h"

#include <vector>

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {add(object);};

        void clear() {objects.clear();}

        void add(shared_ptr<hittable> object) {objects.push_back(object);}

        bool hit(const ray& r, interval t_range, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_something = false;
            auto closest = t_range;
            for (const auto object : objects){
                if (object -> hit(r, closest, temp_rec)){
                    hit_something = true;
                    closest.max = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_something;
        }
};
