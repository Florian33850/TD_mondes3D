#ifndef VIEWER_H
#define VIEWER_H

#include "shader.h"
#include "camera.h"
#include "trackball.h"
#include "mesh.h"

#include <iostream>

class Viewer{
public:
    //! Constructor
    Viewer();
    virtual ~Viewer();

    // gl stuff
    void init(int w, int h);
    void drawScene();
    void drawScene2D();
    void drawScene3DSolarSystem();
    void drawScene3D();
    void updateAndDrawScene();
    void reshape(int w, int h);
    void loadShaders();

    // events
    void mousePressed(GLFWwindow* window, int button, int action);
    void mouseMoved(int x, int y);
    void mouseScroll(double x, double y);
    void keyPressed(int key, int action, int mods);
    void charPressed(int key);

private:
    int _winWidth, _winHeight;

    Camera _cam;
    Shader _shader;
    Mesh   _mesh;
    float _zoom = 1;
    Eigen::Vector2f _offset{0.f,0.f};
    int _wireframe=0;
    float _theta = 0;
    Eigen::Vector3f _vecAxis = Eigen::Vector3f::UnitX();
    Eigen::Vector3f _tAxis{0.5, 0, 0};

    Eigen::Matrix4f M_sun;
    Eigen::Matrix4f M_earth;
    Eigen::Matrix4f M_moon;


    // Mouse parameters for the trackball
    enum TrackMode
    {
      TM_NO_TRACK=0, TM_ROTATE_AROUND, TM_ZOOM,
      TM_LOCAL_ROTATE, TM_FLY_Z, TM_FLY_PAN
    };
    TrackMode _trackingMode = TM_NO_TRACK;
    Trackball _trackball;
    Eigen::Vector2i _lastMousePos;
};

#endif
