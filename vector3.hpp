#include <cmath>

using namespace std;

struct vector3
{
    float x, y, z;
    vector3();
    vector3(float x, float y, float z);

    float magnitude();
    float dot(vector3 vec);
    vector3 normalize();
    
    vector3 operator*(float scalar);
    vector3 operator*(vector3 vec);

    vector3 operator+(vector3 vec);
    vector3 operator-(vector3 vec);
};

