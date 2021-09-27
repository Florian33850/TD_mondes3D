#include "integrator.h"
#include "scene.h"

class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        Hit hit = Hit();
        scene->intersect(ray, hit);
        Color3f r;
        if(hit.foundIntersection()){
            Point3f pIntersect = ray.at(hit.t());
            Normal3f normal = hit.normal();
            Vector3f viewDir = ray.direction;
            viewDir.normalize();
            float epsilon = 0.0001;
            
            for(Light *light : scene->lightList()){
                float dist;
                Vector3f lightDir = light->direction(pIntersect, &dist);
                lightDir.normalize();
                Color3f lightInt = light->intensity(pIntersect);

                float max = lightDir.dot(normal);
                if(max < 0)
                    max = 0;

                Color3f phi = hit.shape()->material()->brdf(viewDir, lightDir, normal);

                Ray rayFromP = Ray(pIntersect+(epsilon*normal), lightDir);
                Hit hit2 = Hit();
                scene->intersect(rayFromP, hit2);

                if(!hit2.foundIntersection() || (hit2.t() > dist && dist != std::numeric_limits<float>::max()))
                    r +=  phi * max * lightInt;
            }

            Vector3f re = 2 * ((-viewDir).dot(normal)) * normal - (-viewDir);
            Ray rebond = Ray(pIntersect+(epsilon*normal), re);
            Hit hitRebond = Hit();
            scene->intersect(rebond, hitRebond);
            rebond.recursionLevel = ray.recursionLevel;
            if(hitRebond.foundIntersection()){
                if(rebond.recursionLevel < 4){
                    rebond.recursionLevel++;
                    r+= Li(scene, rebond)*hit.shape()->material()->reflectivity();
                }
            }
            return r;
        }
        else
            return scene->backgroundColor();
    }
        

    std::string toString() const {
        return "WhittedIntegrator[]";
    }
};

REGISTER_CLASS(WhittedIntegrator, "whitted")
