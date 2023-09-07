#include "camera.hpp"

class World {
    vector<Object> objects;
    Camera camera;

    public:
    World() = default;
    World(float fov, float near, float w, float h, vector3 eye, vector3 center, vector3 up, vector<Object> objects): 
        camera(fov, near, w, h, eye, center, up), objects(objects) {};

    inline World(float fov, float near, float w, float h, vector3 eye, vector3 center, vector3 up): 
        camera(fov, near, w, h, eye, center, up) {objects = vector<Object>();};

    ~World() = default;

    void addObject(Object object);
    
    void render();

};