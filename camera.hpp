#include "object.hpp"
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Camera
{
    float f, w, h, a, b;
    vector3 ze, ye, xe, eye;
    Mat img;

public:
    Camera(): f(0), w(0), h(0), a(0), b(0), ze(vector3()), ye(vector3()), xe(vector3()), eye(vector3()) {};
    Camera(float fov, float near, float w, float h, vector3 eye, vector3 center, vector3 up);
    ~Camera() = default;

    float getFov();
    float getNear();
    float getWidth();
    float getHeight();
    float getA();
    float getB();
    vector3 getZe();
    vector3 getYe();
    vector3 getXe();
    vector3 getEye();

    Ray throwRay(float x, float y);

    Camera initialice(float fov, float near, float w, float h, vector3 eye, vector3 center, vector3 up);
    void render(vector<Object> &objects);

};
