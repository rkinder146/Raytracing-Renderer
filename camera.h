#pragma once

#include "hittable.h"
#include "material.h"

class camera {
    public:
        int image_width = 100;
        double aspect_ratio = 1.0;
        double focal_length = 1.0;
        double viewport_height = 1.0;
        point3 camera_pos = vec3(0,0,0);
        int samples_per_pixel = 100;
        int max_depth = 10;

        camera() {}
        camera(int image_width, double aspect_ratio, point3 camera_pos, double focal_length, double viewport_height, int samples_per_pixel) 
            : image_width(image_width), aspect_ratio(aspect_ratio), camera_pos(camera_pos), focal_length(focal_length), viewport_height(viewport_height), samples_per_pixel(samples_per_pixel) {}

        void render(const hittable& world){
            init();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0.0, 0.0, 0.0);
                    for (int s = 0; s < samples_per_pixel; s++){
                        auto pixel = sampleRay(i, j);
                        
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
        vec3 viewport_u, viewport_v, pixel_delta_u, pixel_delta_v, viewport_upper_left, pixel00pos;
        

        void init() {
            image_height = (int)(image_width/aspect_ratio);
            image_height = image_height > 0 ? image_height : 1;
            
            viewport_width = viewport_height * ((double)image_width/image_height);
            
            // viewport vectors right and down
            viewport_u = vec3(viewport_width, 0, 0);
            viewport_v = vec3(0, -viewport_height,0);

            // pixel ray offset
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // calculate top left of viewport
            viewport_upper_left = camera_pos + vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

            pixel00pos = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
        }

        ray sampleRay(int i, int j) const {
            auto offset = randomOffset();
            auto viewport_pos = (pixel00pos 
                + (pixel_delta_u * (i + offset.x())) 
                + (pixel_delta_v * (j + offset.y())));
                
            return ray(camera_pos, viewport_pos - camera_pos);
        }

        vec3 randomOffset() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0){
                return color(0, 0, 0);
            }
            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)){
                ray scattered_ray;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered_ray));
                    return attenuation * ray_color(scattered_ray, depth - 1, world);
                return color(0,0,0);
                //return 0.5 * color(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0); // normal view
            }

            auto a = unit_vector(r.direction()).y() / 2 + 1;
            return color(1,1,1) * (1-a) + a * color(0.5, 0.6, 0.9);
        }
};