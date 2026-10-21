#pragma once

#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) const {
            return false;
        }
};

class lambertian : public material {
    public: 
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) const override {
            vec3 scatter_dir = rec.normal + random_on_hemisphere(rec.normal);

            if (scatter_dir.near_zero()){
                scatter_dir = rec.normal;
            }

            scattered_ray = ray(r.at(rec.t), scatter_dir);
            attenuation = albedo;
            return true;
        }
    private: 
        color albedo;
};

class metal : public material {
    public: 
        metal(const color& albedo, const double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) const override{
            scattered_ray = ray(rec.p, unit_vector(reflect(r.direction(), rec.normal)) + random_unit_vector() * fuzz);
            attenuation = albedo;
            return dot(rec.normal, scattered_ray.direction()) >= 0;
        }
    private:
        color albedo;
        double fuzz;
};