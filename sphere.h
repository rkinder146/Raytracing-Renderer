#pragma once
#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& pos, double radius) : pos(pos), radius(std::fmax(0,radius)) {}

        bool hit(const ray& r, interval t_range, hit_record& rec) const override {
            vec3 oc = pos - r.origin();
            auto a = dot(r.direction(), r.direction());
            auto h = dot(r.direction(), oc);
            auto c = dot(oc, oc) - radius * radius;
            auto discriminant = h * h - a * c;
            if (discriminant < 0){
                return false;
            }
            auto sqrtd = std::sqrt(discriminant);
            auto root = (h - sqrtd) / a;
            if (!t_range.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!t_range.surrounds(root))
                    return false;
            }
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - pos) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    private:
        point3 pos;
        double radius;
};