#include "Camera.h"
#include <cmath>

using namespace std;

void Camera::initialice(float _near, float fov, int ancho, int h,
                        vector3 eye, vector3 center, vector3 up) {
    this->f = _near;
    this->w = ancho;
    this->h = h;
    this->a = 2 * f * tan(fov * 3.14159265358979323846 / 360);
    
    this->b = w / h * a;
    this->eye = eye;
    this->ze = eye - center;
    this->ze.normalize();
    this->xe = up.cross(ze);
    this->xe.normalize();
    this->ye = ze.cross(xe);
}

void Camera::render(vector<Object *> objects, string name) {

    vector<Object* > lights;
    get_lights(objects, lights);

    Ray rayo;
    rayo.ori = eye;
    vector3 dir;

    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camera Pinhole");

    vector3 color;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color = final_color(rayo, objects, lights, 0);
            (*pImg)(x, h - 1 - y, 0) = (BYTE) (color.x * 255);
            (*pImg)(x, h - 1 - y, 1) = (BYTE) (color.y * 255);
            (*pImg)(x, h - 1 - y, 2) = (BYTE) (color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    string nombre_archivo = name + ".bmp";
    pImg->save(nombre_archivo.c_str());
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }
}


void Camera::get_lights(vector<Object*> objects, vector<Object*> lights){
    for (auto &obj: objects){
        if (obj->is_light){
            lights.emplace_back(obj);
        }
    }
}

vector3 Camera::final_color(Ray rayo, vector<Object *> objects, vector<Object *> lights, int depth) {
    vector3 color(0, 0, 0), normal, normal_temp;
    bool b_obj = false;
    float t = FLT_MAX, t_temp;
    Object *closest;

    //punto de interseccion mas cercano a la camara
    for (auto obj: objects) {
        if (obj->intersect(rayo, t_temp, normal_temp)) {
            b_obj = true;
            if (t_temp < t) {
                t = t_temp;
                normal = normal_temp;
                closest = obj;
            }
        }
    }

    if (b_obj && closest->is_light){
        color = closest->color;
    }
    else if (b_obj) {

        vector3 pi = rayo.ori + rayo.dir * t;
        vector3 V = -rayo.dir; //hacia donde viene el rayo
        float ka = 0.2; // constante de reflexion del ambiente
        vec3 l_amb = vec3(1, 1, 1) * ka;
        
        for (auto &luz: lights){
            
            //vector L hacia la luz
            vec3 L = luz->pos - pi;
            double disL = L.modulo(); //distancia de pi hacia la fuente de luz
            L.normalize();

            //sombra
            bool exists_shadow = false;
            vec3 pis = pi + e * normal; // para que rayo de sombra no se choque con mismo objeto de donde sale
            Rayo rs(pis, L);

            //punto de interseccion mas cercano
            for (auto &obj: objects) {
                if (!obj->light && obj->intersectar(rs, t_temp, normal_temp) && !obj->transparency) {
                    if (t_temp < disL) {
                        exists_shadow = true;//rs choco con un objeto antes de llegar a la luz
                        break;
                    }
                }
            }
        }

        color = closest->color;

        if(!closest->transparency){
            if (b_shadow){
                    color = color * l_amb;
            } else {
                    vector3 l_dif = vector3(0, 0, 0);
                    vector3 l_spec = vector3(0, 0, 0);

                    for(auto& luz: lights){
                        //luz difusa
                        float f_dif = normal.dot(L);// >0
                        if (f_dif > 0)// >0
                            l_dif = luz->color * closest->kd * f_dif;

                        //luz especular
                        vector3 R = 2 * (L.dot(normal)) * normal - L;
                        R.normalize();
                        V.normalize();

                        float f_spec = R.dot(V); // no puede ser negativo ni cero

                        if (f_spec > 0)
                            l_spec = l_spec + luz->color * closest->ks * pow(f_spec, closest->n);
                    }

                    //luz total
                    color = color + closest->color * (l_amb + l_dif/luces.size() + l_spec/luces.size());
                
            }
        }

        if (depth+1 > max_depth) {
            if (closest->transparency){
                vector3 color_refraction(0, 0, 0);

                float kr;
                fresnel(rayo.dir, normal, closest->ior, kr);
                
                bool outside = rayo.dir.dot(normal) < 0;
                vector3 bias = e * normal;

                // compute refraction if it is not a case of total internal reflection
                if (kr < 1) {
                    vector3 refraction_dir = refract(rayo.dir, normal, closest->ior);
                    refraction_dir.normalize();
                    vector3 refraction_ori = outside ? pi - bias : pi + bias;
                    Ray refraction_ray(refraction_ori, refraction_dir);
                    color_refraction = final_color(refraction_ray, objects, luz, depth + 1);
                }
                
                vector3 reflection_dir = 2 * (V.dot(normal)) * normal - V;
                reflection_dir.normalize();
                vector3 reflection_ori = outside ? pi + bias : pi - bias;
                Ray reflection_ray(reflection_ori, reflection_dir);
                vector3 color_reflection = final_color(reflection_ray, objects, luz, depth + 1);

                //reflection + refraction
                color =  color*ka + color_reflection * kr + color_refraction * (1 - kr);

            } else {// no es transparente
                    //recursividad, espejo de colores
                    if (depth+1 < max_depth && closest->ke > 0) {
                        // reflection
                        Ray rr;
                        rr.ori = pi + e * normal;
                        rr.dir = 2 * (V.dot(normal)) * normal - V;
                        rr.dir.normalize();
                        vector3 color_reflection = final_color(rr, objects, luz, depth + 1);
                        color = color + closest->ke * color_reflection;
                    }
                }
                color.max_to_one();
            }
    }
    return color;
}

vector3 Camera::refract(vector3 &I, vector3 &N, float &ior) {
    float cosi = clamp(-1, 1, I.dot(N));
    float etai = 1, etat = ior;
    vector3 n = N;
    if (cosi < 0) { cosi = -cosi; }
    else {
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vector3(0, 0, 0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Camera::fresnel(vector3 &I, vector3 &N, float &ior, float &kr) {
    float cosi = clamp(-1, 1, I.dot(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }

    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}