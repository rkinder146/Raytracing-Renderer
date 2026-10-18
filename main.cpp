#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <iostream>

double hit_sphere(const ray& r, const vec3 center, double radius){
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b -4.0 * a * c;
    if (discriminant < 0){
        return -1.0;
    }
    else {
        return (-b - std::sqrt(discriminant)) / (2.0 * a);
    }
}

color ray_color(const ray& r){
    auto circle_pos = vec3(1, 0, 3);
    auto radius = 1.0;
    auto t = hit_sphere(r, circle_pos, radius);

    if (t > 0.0){
        vec3 N = unit_vector(r.at(t) - circle_pos);
        return 0.5 * color(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
    }
    vec3 unit_dir = unit_vector(r.direction());
    auto a = (unit_dir.y()+1.0)*0.5;
    return color(1.0, 1.0, 1.0) * (1.0-a) + color(0.0, 0.0, 1.0) * a;
}

int main() {

    // Image Setup

    int image_width = 800;
    double aspect_ratio = 16.0 / 9.0;

    int image_height = (int)(image_width/aspect_ratio);
    image_height = image_height > 0 ? image_height : 1;

    // Camera
    auto focal_length = 2.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * ((double)image_width/image_height);
    auto camera_pos = vec3(0,0,0);

    // viewport vectors right and down
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height,0);

    // pixel ray offset
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // calculate top left of viewport
    auto viewport_upper_left = camera_pos + vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

    auto pixel00pos = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel = ray(camera_pos, (pixel00pos + pixel_delta_u * i + pixel_delta_v * j) - camera_pos);
            
            auto pixel_color = ray_color(pixel);
            
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}