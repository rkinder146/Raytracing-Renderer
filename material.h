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

        bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) const override {
            scattered_ray = ray(rec.p, unit_vector(reflect(r.direction(), rec.normal)) + random_unit_vector() * fuzz);
            attenuation = albedo;
            return dot(rec.normal, scattered_ray.direction()) >= 0;
        }
    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered_ray) const override {
            attenuation = color(1.0,1.0,1.0);
            double ri = rec.front_face ? 1.0/refraction_index : refraction_index;
            vec3 unit_dir = unit_vector(r.direction());
            double cos_theta = std::fmin(dot(rec.normal, -unit_dir), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
            if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) {
                scattered_ray = ray(rec.p, reflect(unit_dir, rec.normal));
            }
            else {
                scattered_ray = ray(rec.p, refract(unit_dir, rec.normal, ri));
            }
            return true;
        }
    private:
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 *= r0;
            return r0 + (1 - r0) * std::pow(1-cosine, 5);
        }
};