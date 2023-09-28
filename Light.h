#ifndef RAYTRACING_LUZ_H
#define RAYTRACING_LUZ_H

#include "vector3.h"
#include "Object.h"

class Light : public Sphere{
public:
    vector3 pos, color;
    bool is_visible = false;

    Light(vector3 _pos, vector3 _color) : Sphere(_pos, 0.2, _color, 0) {
        pos = _pos;
        color = _color;
    }
};


#endif
