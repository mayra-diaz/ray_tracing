#include "vector3.h"

vector3 operator*(float f, vector3 v) { return vector3{v.x * f, v.y * f, v.z*f}; }

float clamp(float x, float y, float value) {
    if (value < x) {
        return x;
    }
    if ( value > y) {
        return y;
    }
    return value;
}