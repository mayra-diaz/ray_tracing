#ifndef RAY_H
#define RAY_H

#include "vector3.h"

struct Ray {
    vector3 ori, dir;
    Ray() = default;
    Ray(vector3 o, vector3 d): ori(o), dir(d) {}
};

#endif