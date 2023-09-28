#include <iostream>
#include "camera.h"

std::random_device rd; 
std::mt19937 gen(rd());

float get_random(float a, float b){
    return static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
}

void generate_lights(vector<Object *> & objects, int size);
void generate_spheres(vector<Object *> & objects, int size, bool is_light=true);
void generate_plane(vector<Object *>& objects);

int main(int argc, char* argv[]) {

    vector3 center = vector3(0,0,0);
    vector3 up = vector3(0,1,0);

    Camera cam;
    cam.initialice(3, 100, 700, 700,
                   vector3(0, 20, 50),
                   center,
                   up);

    vector<Object *> objects;
    
    generate_lights(objects, 2);
    generate_plane(objects);
    
    Object * c = new Cilinder(vector3(0, 0, 0), vector3(0, 30, 0), 20, vector3(0.9, 0.9, 0.9));
    c->init_constants(0.7, 0.3, 10, 0, true, 1);
    objects.emplace_back(c);
    vector<Light *> luces;

    cam.render(objects, "imagen");

    return 0;
}

vector3 generate_vector(float a, float b){
    float x = get_random(a, b);
    float y = get_random(a, b);
    float z = get_random(a, b);

    return vector3{x, y, z};
}

void generate_spheres(vector<Object *> & objects, int size, bool is_light){
     int t = 0;
     Sphere* sphere = nullptr;
    for (int i = 0; i < size; ++i) {
        // Radius
        float rad = 0.2;

        //Constants
        float kdt = get_random(0.0, 1.0);
        float kst = get_random(0.0, 1.0);
        int nt = std::uniform_int_distribution<int>(0, 40)(gen);
        float ket = get_random(0.0, 1.0);
        bool transpt = 0;
        float iort = get_random(0.0, 1.0);

        vector3 cen = vector3{get_random(5,10), get_random(15.0, 25.0), get_random(10, 30)};
        vector3 col = generate_vector(0.1, 1.0);
        sphere = new Sphere(cen, rad, col);
        sphere->init_constants(kdt, kst, nt, ket, transpt, iort);

        if (is_light){
            sphere->make_light();
        }

        objects.push_back(sphere);
        if(transpt) t++;
    }
}

void generate_lights(vector<Object *> & objects, int size){
     int t = 0;
     Sphere* sphere = nullptr;
    for (int i = 0; i < size; ++i) {
        // Radius
        float rad = 0.2;

        //Constants
        float kdt = 0.7;
        float kst = 0.3;
        int nt = 0;
        float ket = 0;
        bool transpt = 0;
        float iort = 1;

        vector3 cen = vector3{get_random(5,10), get_random(15.0, 25.0), get_random(10, 30)};
        vector3 col = vector3{1.f,0.843137f, 0.f};
        sphere = new Sphere(cen, rad, col);
        sphere->init_constants(kdt, kst, nt, ket, transpt, iort);
        sphere->make_light();

        objects.push_back(sphere);
        if(transpt) t++;
    }
}

void generate_plane(vector<Object* >& objects){
    Object *p1 = new Plane(vector3(0, 1, 0), 1, vector3(0.6, 0.8, 0.6));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0;
    objects.emplace_back(p1);

    Object *p2 = new Plane(vector3(0, 0, -100), 1, vector3(0.6, 0.8, 0.6));
    p2->init_constants(0.9, 0.1);
    p2->ke = 0;
    objects.emplace_back(p2);
}
