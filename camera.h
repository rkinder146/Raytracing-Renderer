#pragma once

#include "hittable.h"

class camera {
    public:
        int image_width;
        double aspect_ratio, focal_length, viewport_height;
        point3 camera_pos;

        camera(int image_width, double aspect_ratio, point3 camera_pos, double focal_length, double viewport_height) 
            : image_width(image_width), aspect_ratio(aspect_ratio), camera_pos(camera_pos), focal_length(focal_length), viewport_height(viewport_height) {}

        // int image_width = 800;
        // double aspect_ratio = 16.0 / 9.0;

        // int image_height = (int)(image_width/aspect_ratio);
        // image_height = image_height > 0 ? image_height : 1;


        // double focal_length = 2.0;
        // double viewport_height = 2.0;
        // double viewport_width = viewport_height * ((double)image_width/image_height);
        // point3 camera_pos = point3(0,0,0);

        void render(const hittable& world){
            init();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
                for (int i = 0; i < image_width; i++) {
                    auto pixel = ray(camera_pos, (pixel00pos + pixel_delta_u * i + pixel_delta_v * j) - camera_pos);
                    
                    auto pixel_color = ray_color(pixel, world);
                    
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

        color ray_color(const ray& r, const hittable& world) const {
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)){
                return 0.5 * color(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0);
            }

            return color(0.9,0.9,0.9);
        }
};