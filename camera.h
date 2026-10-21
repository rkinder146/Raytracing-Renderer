#pragma once

#include "hittable.h"
#include "material.h"

class camera {
    public:
        int image_width = 100;
        double fov = 90;
        double aspect_ratio = 1.0;
        int samples_per_pixel = 100;
        int max_depth = 10;

        point3 camera_pos = vec3(0,0,0);
        point3 look_at = vec3(0,0,1);
        vec3 up = vec3(0,1,0);

        double defocus_angle = 0.0;
        double focus_dist = 1.0;

        camera() {}
        // camera(int image_width, double aspect_ratio, point3 camera_pos, double fov, int samples_per_pixel) 
        //     : image_width(image_width), aspect_ratio(aspect_ratio), camera_pos(camera_pos), fov(fov), samples_per_pixel(samples_per_pixel) {}

        void render(const hittable& world){
            init();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0.0, 0.0, 0.0);
                    for (int s = 0; s < samples_per_pixel; s++){
                        auto pixel = sample_ray(i, j);
                        
                        pixel_color += ray_color(pixel, max_depth, world);
                    }
                    pixel_color *= 1.0 / samples_per_pixel;
                    write_color(std::cout, pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }
    private:
        int image_height;
        double viewport_width;
        vec3 viewport_u, viewport_v, pixel_delta_u, pixel_delta_v, viewport_upper_left, pixel00pos, defocus_u, defocus_v;

        void init() {
            image_height = (int)(image_width/aspect_ratio);
            image_height = image_height > 0 ? image_height : 1;

            auto theta = degrees_to_radians(fov);
            auto viewport_height = 2 * std::tan(theta/2) * focus_dist;
            
            viewport_width = viewport_height * ((double)image_width/image_height);
            
            // get unit vector of camera direction
            auto view_dir_w = unit_vector(look_at - camera_pos);
            auto view_dir_u = -unit_vector(cross(up, view_dir_w));
            auto view_dir_v = cross(view_dir_w, view_dir_u);

            // viewport vectors right and down
            viewport_u = view_dir_u * viewport_width;
            viewport_v = view_dir_v * viewport_height;

            // pixel ray offset
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // calculate top left of viewport
            viewport_upper_left = camera_pos + view_dir_w * focus_dist - viewport_u/2 - viewport_v/2;

            pixel00pos = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_u = view_dir_u * defocus_radius;
            defocus_v = view_dir_v * defocus_radius;
        }

        ray sample_ray(int i, int j) const {
            auto ray_origin = (defocus_angle <= 0) ? camera_pos : camera_pos + random_lens_offset();
            auto viewport_offset = randomOffset();
            auto viewport_pos = (pixel00pos 
                + (pixel_delta_u * (i + viewport_offset.x())) 
                + (pixel_delta_v * (j + viewport_offset.y())));
            return ray(ray_origin, viewport_pos - ray_origin);
        }

        vec3 randomOffset() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        vec3 random_lens_offset() const {
            vec3 rand_disc = random_on_unit_circle();
            return defocus_u * rand_disc[0] + defocus_v * rand_disc[1];
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0){
                return color(0, 0, 0);
            }
            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)){
                ray scattered_ray;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered_ray))
                    return attenuation * ray_color(scattered_ray, depth - 1, world);
                return color(0,0,0);
                //return 0.5 * color(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0); // normal view
            }

            auto a = (unit_vector(r.direction()).y() + 1) / 2;
            return color(0.8,0.8,1) * (1-a) + a * color(0.3, 0.4, 1);
        }
};