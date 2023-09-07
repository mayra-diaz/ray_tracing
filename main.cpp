#include <iostream>
#include "world.hpp"

int main() {

    std::cout << "HOLA\n";

    World world(3, 60, 800, 600,
                vector3(0,5,50),
                vector3(0,0,0),
                vector3(0,1,0));


    for(int i = 0; i < 50; i++){
        float x = (float)rand() / RAND_MAX * 100 - 50;
        float y = (float)rand() / RAND_MAX * 100 - 50;
        float z = (float)rand() / RAND_MAX * 100 - 50;
        float r = rand() %255;
        float g = rand() %255;
        float b = rand() %255;
        float radius = (float)rand() / RAND_MAX * 10;

        vector3 color(r,g,b);
        vector3 position(x,y,z);
        Sphere sphere(radius, position, color);
        world.addObject(sphere);
    }

    world.render();

    return 0;
}