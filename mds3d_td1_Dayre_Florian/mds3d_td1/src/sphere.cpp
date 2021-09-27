#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    /// TODO: compute ray-sphere intersection
    float A = 1;
    float B = 2 * (ray.origin-this->m_center).dot(ray.direction);
    float C = (ray.origin-this->m_center).squaredNorm() - (this->m_radius*this->m_radius);
    float bCarre = B * B;
    float delta = bCarre - 4 * A * C;
    if(delta < 0)
        return false;
    else{
        float racine1;
        float racine2;
        racine1 = (-B - sqrt(delta))/2*A;
        racine2 = (-B + sqrt(delta))/2*A;

        if(racine1 >= 0){
            hit.setT(racine1);
            hit.setShape(this);
            //2.1
            Point3f pIntersect = ray.origin + racine1*ray.direction;
            Normal3f normal = pIntersect - this->m_center;
            normal.normalize();
            hit.setNormal(normal);
        }
        else if(racine2 >= 0){
            hit.setT(racine2);
            hit.setShape(this);
            //2.1
            Point3f inter = ray.origin + racine2*ray.direction;
            Normal3f normal = inter - this->m_center;
            normal.normalize();
            hit.setNormal(normal);
        }
        return true;
    }
}

REGISTER_CLASS(Sphere, "sphere")
