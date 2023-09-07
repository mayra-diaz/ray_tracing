#include "world.hpp"

void World::addObject(Object object){
    objects.push_back(object);
}

void World::render(){
    camera.render(objects);
}
