#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    float t;
    Point3f v = ray.origin - this->m_position;
    Normal3f normal =  this->m_normal;
    normal.normalize();
    t = ((-v).dot(normal)) / ray.direction.dot(normal);
    if(t>=0){
        hit.setT(t);
        hit.setShape(this);
        hit.setNormal(normal);
        return true;
    }
    return false;
}

REGISTER_CLASS(Plane, "plane")
