#include <iostream>
#include "camera.h"

std::random_device rd; 
std::mt19937 gen(rd());
#define mpi 3.14159265358979323846

float get_random(float a, float b){
    return static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
}

void generate_lights(vector<Object *> & objects, int size);
void generate_spheres(vector<Object *> & objects, int size);
void generate_triangles(vector<Object *> & objects);
void generate_plane(vector<Object *>& objects);

vector3 generate_vector(float a, float b){
    float x = get_random(a, b);
    float y = get_random(a, b);
    float z = get_random(a, b);

    return vector3{x, y, z};
}

int main(int argc, char* argv[]) {

    vector3 center = vector3(0,0,0);
    vector3 up = vector3(0,1,0);

    Camera cam;
    cam.initialice(3, 100, 700, 500,
                   vector3(0, 30, 80),
                   center,
                   up);

    vector<Object *> objects;
    
    generate_lights(objects, 5);
    //generate_spheres(objects, 2);
    generate_triangles(objects);

    generate_plane(objects);

    float rad = get_random(5,20);
    float kdt = get_random(0.0, 1.0);
    float kst = get_random(0.0, 1.0);
    int nt = std::uniform_int_distribution<int>(0, 40)(gen);
    float ket = get_random(0.0, 1.0);
    bool transpt = 0;
    float iort = get_random(0.0, 1.0);

    vector3 cen = vector3{get_random(-10,10), get_random(-15.0, 25.0), get_random(10, 30)};
    vector3 col = generate_vector(0.1, 1.0);
    auto sphere = new Sphere(cen, rad, col);
    sphere->init_constants(kdt, kst, nt, ket, transpt, iort);
    objects.push_back(sphere);

    rad = get_random(5,20);
    kdt = get_random(0.0, 1.0);
    kst = get_random(0.0, 1.0);
    nt = std::uniform_int_distribution<int>(0, 40)(gen);
    ket = get_random(0.0, 1.0);
    transpt = 0;
    iort = get_random(0.0, 1.0);

    cen = vector3{get_random(60,100), get_random(-20, 25.0), get_random(-30, 0)};
    col = generate_vector(0.1, 1.0);
    auto sphere2 = new Sphere(cen, rad, col);
    sphere2->init_constants(kdt, kst, nt, ket, transpt, iort);
    objects.push_back(sphere2);
    
    Object * c = new Cilinder(vector3(0, 0, 0), vector3(0, 30, 0), 20, vector3(0.9, 0.9, 0.9));
    c->init_constants(0.7, 0.3, 10, 0, true, 1);
    objects.emplace_back(c);

    Object * c1 = new Cilinder(vector3(-50, 0, 25), vector3(-45, 20, 40), 20, vector3(0.4, 0.4, 0.7));
    c1->init_constants(0.7, 0.3, 10, 0, true, 1);
    objects.emplace_back(c1);
    Object * c2 = new Cilinder(vector3(85, 10, 5), vector3(90, 40, 10), 20, vector3(0.4, 0.4, 0.7));
    c2->init_constants(0.7, 0.3, 10, 0, true, 1);
    objects.emplace_back(c2);

    float posX = 150;
    float posY = 120;

    
    float posZ = 50;
    center = vector3(0, 0, 0);
    int posYDirection = 1; // dir
    float posYIncrement = 3.0f;
    float rotationSpeed = 3.0f;
    int nframes = 720;

    for (int frame = 0; frame < nframes; ++frame) {

        for (auto obj: objects) {
            if (obj->is_light) {
                obj->center.x = 10 + 10 * cos(2 * mpi * 0.2 * frame + 0.3);
                obj->center.z = 10 + 10 * sin(2 * mpi * 0.2 * frame + 0.3);
            }

        }


        float angle = rotationSpeed * 2 * mpi * frame / nframes; // rad
        float radius = sqrt(pow(posX, 2) + pow(posZ, 2)); 

        posX = radius * cos(angle);
        posZ = radius * sin(angle);

        posY += posYDirection * posYIncrement;

        if (posYDirection == 1 && posY >= 140) {
            posYDirection = -1; // Switch to moving down
        } else if (posYDirection == -1 && posY <= 100) {
            posYDirection = 1; // Switch to moving up
        }

        cam.initialice(3, 100, 700, 500,
                       vector3(posX, posY, posZ), // Update eye position
                       center,
                       vector3(0, 1, 0));

        cam.render(objects, "frames/"+to_string(frame+1));
    }

    return 0;
}


void generate_spheres(vector<Object *> & objects, int size){
     Sphere* sphere = nullptr;
    for (int i = 0; i < size; ++i) {
        float rad = 1;
        float kdt = get_random(0.0, 1.0);
        float kst = get_random(0.0, 1.0);
        int nt = std::uniform_int_distribution<int>(0, 40)(gen);
        float ket = get_random(0.0, 1.0);
        bool transpt = 0;
        float iort = get_random(0.0, 1.0);

        vector3 cen = vector3{get_random(-10,10), get_random(-15.0, 25.0), get_random(10, 30)};
        vector3 col = generate_vector(0.1, 1.0);
        sphere = new Sphere(cen, rad, col);
        sphere->init_constants(kdt, kst, nt, ket, transpt, iort);

        objects.push_back(sphere);
    }
}

void generate_triangles(vector<Object *> & objects){
    Triangle* trng = nullptr;
    vector3 x = vector3(20,20,20);
    vector3 y = vector3(30,30,30);
    vector3 z = vector3(25,25,25);
    vector3 c = vector3(0.5, 0.7, 0.4);
    trng = new Triangle(x, y, z, c);
    objects.push_back(trng);
}

void generate_lights(vector<Object *> & objects, int size){
    Sphere* sphere = nullptr;
    float x = 0;
    for (int i = 0; i < size; ++i) {
        //vector3 cen = vector3{get_random(5,10), get_random(15.0, 25.0), get_random(10, 30)};
        vector3 cen = vector3{-5+x, 40, 0};
        vector3 col = vector3{1.f,0.843137f, 0.f};
        sphere = new Sphere(cen, 0.5, col);
        sphere->init_constants(0.7, 0.3, 0, 0, 0, 1);
        sphere->make_light();

        objects.push_back(sphere);
        x += 5;
    }
}

void generate_plane(vector<Object* >& objects){
    Object *p1 = new Plane(vector3(0, -100, 0), 1, vector3(0.6, 0.8, 0.6));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0;
    objects.emplace_back(p1);
}
