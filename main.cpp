#include "helpers.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <iostream>

int main() {
    // Create Camera
    camera cam(400, 16.0/9.0, point3(0, 0, 0), 2.0, 2.0, 100);

    // Environment
    hittable_list world;
    world.add(make_shared<sphere>(point3(1, -1.5, 5), 1.5));
    world.add(make_shared<sphere>(point3(1, -50, 40), 49.5));
    
    // Render
    cam.render(world);
}