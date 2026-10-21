#include "helpers.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>

int main() {
    // Create Camera
    camera cam;        
    cam.image_width = 400;
    cam.aspect_ratio = 16.0/9.0;
    cam.focal_length = 2.0;
    cam.viewport_height = 2.0;
    cam.camera_pos = vec3(0,0,0);
    cam.samples_per_pixel = 100;
    cam.max_depth = 30;


    camera cam2;

    cam2.aspect_ratio      = 16.0 / 9.0;
    cam2.image_width       = 400;
    cam2.samples_per_pixel = 1000;
    cam2.max_depth         = 50;

    
    // Environment
    hittable_list world;
    // world.add(make_shared<sphere>(point3(1, -1.5, 10), 2));
    // world.add(make_shared<sphere>(point3(1, -51, 10), 47.5));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.8);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, 1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, 1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, 1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, 1.0),   0.5, material_right));
    
    // Render
    // cam.render(world);
    cam2.render(world);
}