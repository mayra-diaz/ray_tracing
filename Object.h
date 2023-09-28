#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"
#include "Ray.h"

using namespace std;

class Object {
    public:
    vector3 color;
    float kd; //constante difusa del material [0,1]
    float ks; //constante especular del material [0,1]
    float n; //shininess exponent
    float ke; // constante material espejo/reflexion [0,1>, nunca se refleja el 100%
    bool transparency; 
    float ior; // index of refraction

    bool is_light = false;

public:
    Object() = default;
    Object(vector3 col, float kd = 1) : color{col}, kd{kd} {
        init_constants(kd);
    }

    void init_constants(float kd_ = 1, float ks_ = 1, float n_ = 10, float ke_ = 0, bool transparency_ = false, float ior_ = 1) {
        this->kd = kd_;
        this->ks = ks_;
        this->n = n_;
        this->ke = ke_;
        this->transparency = transparency_;
        this->ior = ior_;
    }

    virtual bool intersect(Ray ray, float &t, vector3 &normal) = 0;

    void make_light(){
        this->is_light = true;
    }
};

class Sphere : public Object {

    vector3 center;
    float radius;

public:
    Sphere(vector3 center, float radius, vector3 col, float kd = 1) :  Object(col, kd), center{center}, radius{radius} {}

    bool intersect(Ray ray, float &t, vector3 &normal) override {
        vector3 d = ray.dir;
        vector3 o = ray.ori;

        float a = d.dot(d);
        float b = 2 * d.dot(o - center);
        float c = (o - center).dot(o - center) - radius * radius;
        
        float temp = b * b - 4 * a * c;
        if (temp <= 0) { return false; }
        float t1 = (-b + sqrt(temp)) / (2 * a);
        float t2 = (-b - sqrt(temp)) / (2 * a);
        
        t = min(t1, t2);
        if (t <= 0) { return false; }

        vector3 pi = ray.ori + t * ray.dir;
        normal = pi - center;
        normal.normalize();

        return true;
    }
};

class Plane : public Object {
public:
    vector3 pnormal;
    float d;

    Plane(vector3 normal, float d, vector3 col) : pnormal(normal), d(d), Object(col) {
        pnormal.normalize();
    }

    bool intersect(Ray ray, float &t, vector3 &normal) override {
        float dirn = ray.dir.dot(pnormal);
        if (dirn != 0) {
            vector3 p1 = pnormal * d; 
            t = (p1 - ray.ori).dot(pnormal) / dirn;
            if (t <= 0) return false; 
            normal = pnormal;
            return true;

        }
        return false;
    }
};

class Triangle : public Object {
public:
    vector3 p1;
    vector3 p2;
    vector3 p3;

    Triangle(vector3 v1, vector3 v2, vector3 v3, vector3 col, float kd = 1) : p1(v1), p2(v2), p3(v3),
                                                                   Object(col, kd) {}

    bool intersect(Ray ray, float &t, vector3 &normal) override {
        vector3 n = (p2 - p1).cross(p3 - p2);
        n.normalize();
        float den = ray.dir.dot(n);
        if (den != 0) {
            t = (p1 - ray.ori).dot(n) / den;
            if (t <= 0) return false; // or just t < 0????
            normal = n;
            //revisar si dot esta dentro del triangulo
            vector3 pint = ray.ori + t * ray.dir;
            if (n.dot((p2 - p1).cross(pint - p1)) > 0 && n.dot((p3 - p2).cross(pint - p2)) > 0 &&
                n.dot((p1 - p3).cross(pint - p3)) > 0)
                return true;

        }
        return false;
    }
};

class Cilinder : public Object {
public:
    vector3 pa, pb;
    float radius;

    Cilinder(vector3 pa_, vector3 pb_, float rad, vector3 col, float kd = 1) : pa(pa_), pb(pb_), radius(rad), Object(col, kd) {}

    bool intersect(Ray ray, float &t, vector3 &normal) override {
        vector3 ro = ray.ori;
        vector3 rd = ray.dir;
        vector3 ba = pb - pa;
        vector3 oc = ro - pa;

        float baba = ba.dot(ba);
        float bard = ba.dot(rd);
        float baoc = ba.dot(oc);

        float k2 = baba - bard * bard;
        float k1 = baba * oc.dot(rd) - baoc * bard;
        float k0 = baba * oc.dot(oc) - baoc * baoc - radius * radius * baba;

        float h = k1 * k1 - k2 * k0;
        if (h < 0.0) return false;

        //body
        h = sqrt(h);
        t = (-k1 - h) / k2;
        if (t <= 0) return false;
        float y = baoc + t * bard;
        if (y > 0.0 && y < baba) {
            normal = (oc + t * rd - ba * y / baba) / radius;
            normal.normalize();
            return true;
        }
        // caps
        t = (((y < 0.0) ? 0.0 : baba) - baoc) / bard;
        if (abs(k1 + k2 * t) < h) {
            normal = ba * sgn(y) / baba;
            normal.normalize();
            return true;
        }
        return false;

    }
};

#endif
