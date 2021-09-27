#include "integrator.h"
#include "scene.h"
#include "math.h"

class NormalsIntegrator : public Integrator {
public:
    NormalsIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */
        Hit hit = Hit();
        scene->intersect(ray, hit);
        if(hit.foundIntersection()){
            float c1, c2, c3;
            c1 = hit.normal()[0];
            c1 = fabs(c1);
            
            c2 = hit.normal()[1];
            c2 = fabs(c2);

            c3 = hit.normal()[2];
            c3 = fabs(c3);

            Color3f color = {c1, c2, c3};
            return color;
        }
        else
            return scene->backgroundColor();
    }

    std::string toString() const {
        return "NormalsIntegrator[]";
    }
};

REGISTER_CLASS(NormalsIntegrator, "normals")
