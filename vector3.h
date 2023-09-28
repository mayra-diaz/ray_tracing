#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <algorithm>
#include <iostream>

template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct vector3 {
    float x=0, y=0, z=0;
    
    vector3() = default;

    vector3(float x, float y, float z): x(x), y(y), z(z) {}

    vector3 cross(vector3 v) { return vector3(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y); }

    float dot(vector3 v) { return x * v.x + y * v.y + z * v.z; }

    vector3 operator+(vector3 v) { return vector3(x + v.x, y + v.y, z + v.z); }

    vector3 operator*(vector3 v) { return vector3(x * v.x, y * v.y, z * v.z); }

    vector3 operator-(vector3 v) { return vector3(x - v.x, y - v.y, z - v.z); }

    bool operator==(vector3 v) { return (x == v.x) && (y == v.y) && (z == v.z); }

    vector3 operator-() { return vector3(-x, -y, -z); }

    vector3 operator*(float f) { return vector3(f * x, f * y, f * z); }

    vector3 operator/(float f) { return vector3(x / f, y / f, z / f); }

    void operator*=(vector3 v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    void normalize() {
        double m = modulo();
        x = x / m;
        y = y / m;
        z = z / m;
    }

    double modulo() {
        return sqrt(x * x + y * y + z * z);
    }

    void max_to_one() {
        float max_value = std::max(x, std::max(y, z));
        if (max_value > 1.0) {
            x = x / max_value;
            y = y / max_value;
            z = z / max_value;
        }
    }
};

float clamp(float menor, float mayor, float valor);

vector3 operator*(float f, vector3 v);
#endif