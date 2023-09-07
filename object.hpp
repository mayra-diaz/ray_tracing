#include "ray.hpp"
#include <algorithm>

class Object
{
private:
    vector3 color;
    
public:
    Object() = default;
    Object(vector3 color): color(color) {};
    ~Object() = default;

    inline vector3 getColor(){return this->color;};

    virtual bool intersect(Ray ray, float &t){return false;};
};

class Sphere: public Object
{
    float radius;
    vector3 center;

public:
    Sphere() = default;
    Sphere(float radius, vector3 center, vector3 color);
    ~Sphere() = default;

   bool intersect(Ray ray, float &t);
};

