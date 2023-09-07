#include "camera.hpp"


Camera Camera::initialice(float fov, float near, float w, float h, vector3 eye, vector3 center, vector3 up){
    this->w = w;
    this->h = h;
    this->f = near;

    this->a = 2*f*tan(fov / 2.0f);
    this->b = w/h * a;

    this->ze = (eye - center)* (1/(eye - center).magnitude());
    this->xe = (up * ze) * (1/(up * ze).magnitude());
    this->ye = ze * xe;

    return *this;
}

Ray Camera::throwRay(float x, float y){
    vector3 dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
    Ray ray = Ray(eye, dir);
    return ray;
}

void Camera::render(vector<Object>& objects){

    img = Mat(480, 640, CV_8UC3, Scalar(0,0,0));
    float t = 0;
    vector3 pixel(0,0,0);

    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            pixel = vector3(0,0,0);
            Ray ray = throwRay(i, j);
            ray.direction = ray.direction.normalize();
            for (auto object : objects)
            {
                float t = 0;
                if (object.intersect(ray, t))
                {
                    pixel = object.getColor();
                }
                img.at<Vec3b>(i, j) = Vec3b(pixel.x * 255, pixel.y * 255, pixel.z * 255);
            }
            
        }
        
    }
    
    imwrite("img1.png", img);
    imshow("Imagen", img);
    waitKey(0);
    destroyAllWindows();
}

float Camera::getFov()
{
    return this->f;
}

float Camera::getNear()
{
    return this->f;
}

float Camera::getWidth()
{
    return this->w;
}

float Camera::getHeight()
{
    return this->h;
}

float Camera::getA()
{
    return this->a;
}

float Camera::getB()
{
    return this->b;
}

vector3 Camera::getZe()
{
    return this->ze;
}

vector3 Camera::getYe()
{
    return this->ye;
}

vector3 Camera::getXe()
{
    return this->xe;
}

vector3 Camera::getEye()
{
    return this->eye;
}
