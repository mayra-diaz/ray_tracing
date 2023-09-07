#include "object.hpp"

bool Sphere::intersect(Ray ray, float &t){
    vector3 d = ray.direction;
    vector3 o = ray.origin;
    float a = d.dot(d);
    float b = 2 * d.dot(o - this->center);
    float c = (o - this->center).dot(o - this->center) - this->radius * this->radius;

    if (b*b - 4*a*c > 0) {return true;}
    
    float t1 = (-b - sqrt(b*b - 4*a*c))/(2*a);
    float t2 = (-b + sqrt(b*b - 4*a*c))/(2*a);
    t = min(t1, t2);

    if (t < 0) {return false;}

    return true;

}