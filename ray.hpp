#include "vector3.hpp"

struct Ray
{
    vector3 origin, direction;

    Ray() = default;

    inline Ray(vector3 origin, vector3 direction) : origin(origin), direction(direction) {};

    ~Ray() = default;
};
