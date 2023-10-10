//
// Created by hgallegos on 30/03/2023.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "vector3.h"
#include "CImg.h"
#include "Object.h"
#include "Light.h"

#include <vector>
#include <random>

using namespace std;
using namespace cimg_library;
typedef unsigned char BYTE;

class Camera {
    vector3 eye, xe, ye, ze;
    float f, a, b, w, h;
    CImg<BYTE> *pImg = nullptr;
    int max_depth=5;

    void get_lights(vector<Object*> objs, vector<Object*>& lights);

public:
    Camera() = default;

    void initialice(float _near, float fov, int ancho, int alto,
                    vector3 pos_eye, vector3 center, vector3 up);

    void render(vector<Object *> objects, string name = "x");

    vector3 final_color(Ray rayo, vector<Object *> objects, vector<Object*>& luz, int depth);

    vector3 refract(vector3 &I, vector3 &N, float &ior);

    void fresnel(vector3 &I, vector3 &N, float &ior, float &kr);
};


#endif 