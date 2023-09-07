#include "vector3.hpp"

vector3::vector3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

vector3::vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float vector3::magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

float vector3::dot(vector3 vec)
{
    return x * vec.x + y * vec.y + z * vec.z;
}

vector3 vector3::normalize()
{
    float mod = sqrt(x * x + y * y + z * z);
    return vector3(x / mod, y / mod, z / mod);
}

vector3 vector3::operator*(float scalar)
{
    return vector3(x * scalar, y * scalar, z * scalar);
}

vector3 vector3::operator*(vector3 vec)
{
    return vector3(x * vec.x, y * vec.y, z * vec.z);
}

vector3 vector3::operator+(vector3 vec)
{
    return vector3(x + vec.x, y + vec.y, z + vec.z);
}

vector3 vector3::operator-(vector3 vec)
{
    return vector3(x - vec.x, y - vec.y, z - vec.z);
}