#include "viewer.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
  : _winWidth(0), _winHeight(0)
{
}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h){
    loadShaders();

    // if(!_mesh.load(DATA_DIR"/models/quad.off")) exit(1);
    // if(!_mesh.load(DATA_DIR"/models/lemming.off")) exit(1);
    if(!_mesh.load(DATA_DIR"/models/sphere.obj")) exit(1);
    // if(!_mesh.load(DATA_DIR"/models/monkey.obj")) exit(1);
    _mesh.computeNormals();
    _mesh.initVBA();

    reshape(w,h);
    _trackball.setCamera(&_cam);

    glClearColor(0.5,0.5,0.5,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    _cam.setPerspective(float(M_PI/2), 0.5f, 1e6f);
    _cam.lookAt(Vector3f(1,1,1), Vector3f(0,0,0), Vector3f(0,1,0));

    // Initialisation de la matrice earth
    M_earth.setIdentity();
    //AngleAxisf pour l'inclinaison de la terre, Scalling pour la taille
    Affine3f affine_earth = Translation3f(2,0,0) * AngleAxisf(23.44 * M_PI /180 , Vector3f::UnitZ()) * Scaling(Vector3f(0.4,0.4,0.4));
    M_earth = affine_earth.matrix();

    // Initialisation de la matrice moon
    M_moon.setIdentity();
    Affine3f t_moon = Translation3f(2.7,0,0) * AngleAxisf(23.44 * M_PI /180 , Vector3f::UnitZ()) * Scaling(Vector3f(0.15,0.15,0.15));
    M_moon = t_moon.matrix();

    //translation earth/moon
    Vector3f t_moon_earth = Affine3f(M_earth) * Vector3f(0,0,0);
    t_moon =  Translation3f(t_moon_earth) * AngleAxisf(5.14 * M_PI /180,  Vector3f::UnitX()) * Translation3f(-t_moon_earth) ;
    M_moon = t_moon.matrix() * M_moon;

    // Initialisation de la matrice sun
    M_sun.setIdentity();
    Affine3f affine_sun = Translation3f(0,0,0) * Scaling(Vector3f(1,1,1));
    M_sun = M_sun * affine_sun.matrix();

}

void Viewer::reshape(int w, int h){
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w,h);
}


/*!
   callback to draw graphic primitives
 */
void Viewer::drawScene()
{
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();

  glUniform1i(_shader.getUniformLocation("wireframe"), 0);

  _mesh.draw(_shader);

  if(_wireframe)
  {
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDepthFunc(GL_LEQUAL);

    _mesh.draw(_shader);
    
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthFunc(GL_LESS);
  }

  _shader.deactivate();
}

void Viewer::drawScene2D()
{
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);

   Matrix4f M;

  //2) Transformation X-Y
   //2)
    // M <<  _zoom, 0, 0, _offset.x(),
    //       0, _zoom, 0, _offset.y(),
    //       0, 0, 1, 0,
    //       0, 0, 0, 1;

    //3.1) Chaise posée au sol
    M <<  0.5, 0, 0, -0.5,
          0, 0.5, 0, -1,
          0, 0, 1, 0,
          0, 0, 0, 1;
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M.data());
  _mesh.draw(_shader);

  //3.2) 2 chaises posées au sol
   M <<  -0.5, 0, 0, 0.5,
          0, 0.5, 0, -1,
          0, 0, 0.5, 0,
          0, 0, 0, 1;
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M.data());
  _mesh.draw(_shader);

  //3.3) Troisième chaise et rotation
  Matrix4f R;
  //  R <<  cos(_theta), -sin(_theta), 0, 0,
  //         sin(_theta), cos(_theta), 0, 0,
  //         0, 0, 1, 0,
  //         0, 0, 0, 1;
  //   glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, R.data());
  // _mesh.draw(_shader);

  //3.4) rotation autour du centre de gravité
  Matrix4f T1, T2;
  //Translation de 0,5 en y
    T1 <<  1, 0, 0, 0,
          0, 1, 0, 0.5,
          0, 0, 1, 0,
          0, 0, 0, 1;
  
  //Rotation
    R <<  cos(_theta), -sin(_theta), 0, 0,
          sin(_theta), cos(_theta), 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;
    
    //Translation de -0,5 en y
    T2 <<  1, 0, 0, 0,
          0, 1, 0, -0.5,
          0, 0, 1, 0,
          0, 0, 0, 1;

    R = T1*R*T2;
  //   glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, R.data());
  // _mesh.draw(_shader);

  //4.1)
  Affine3f A = Translation3f(_tAxis)
              * AngleAxisf(_theta, _vecAxis)
              * Translation3f(-_tAxis);
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, A.matrix().data());
  _mesh.draw(_shader);

  if(_wireframe)
  {
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDepthFunc(GL_LEQUAL);

    _mesh.draw(_shader);
    
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthFunc(GL_LESS);
  }

  _shader.deactivate();
}

void Viewer::drawScene3DSolarSystem()
{
  
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();
  glUniform1i(_shader.getUniformLocation("wireframe"), 0);

  //4)Système solaire    
    // Sun
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //Shader de sun
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M_sun.data());
    glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
    glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

    glUniform3f(_shader.getUniformLocation("light_pos"), 5.0, 1.0, 5.0);
    
    Eigen::Matrix3f normal;
    normal= ((_cam.viewMatrix().topLeftCorner(3,3) * M_sun.topLeftCorner(3,3)).inverse()).transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, normal.data());
    _mesh.draw(_shader);

    //  Earth
    // Rotation
    //Permet d'obtenir le vecteur de translation
    Vector3f vector_earth = Affine3f(M_earth) * Vector3f(0,0,0); 
     // Translation + Rotation en augmentant l'angle de 0.01 sur l'axe y + Translation inverse
    Affine3f affine_earth = Translation3f(vector_earth) * AngleAxisf(0.2, Vector3f::UnitY()) * Translation3f(-vector_earth); 
    // Mise en place de la rotation
    M_earth = affine_earth.matrix() * M_earth; 

    // Rotation autour du soleil
    // Rotation de la même manière que précédemment, pas de translation inverse car la rotation est autour d'un autre objet
    affine_earth = AngleAxisf(0.01, Vector3f::UnitY()); 
    //application de la rotation
    M_earth = affine_earth.matrix() * M_earth; 

    //Shader de earth
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M_earth.data());
    glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
    glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

    glUniform3f(_shader.getUniformLocation("light_pos"), 5.0, 1.0, 5.0);

    normal= ((_cam.viewMatrix().topLeftCorner(3,3) * M_earth.topLeftCorner(3,3)).inverse()).transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, normal.data());
    _mesh.draw(_shader);

    // Moon
    // Rotation autour d'elle même
    Vector3f vector_moon = Affine3f(M_moon) * Vector3f(0,0,0);
    Affine3f affine_moon = Translation3f(vector_moon) * AngleAxisf(0.2,Vector3f::UnitY()) * Translation3f(-vector_moon);
    M_moon=affine_moon.matrix()*M_moon;

    // Rotation autour du soleil
    affine_moon = AngleAxisf(0.01, Vector3f::UnitY());
    M_moon = affine_moon.matrix() * M_moon;

    //Rotation autour de la terre
    vector_moon = Affine3f (M_earth) * Vector3f(0,0,0);
    affine_moon= Translation3f(vector_moon) * AngleAxisf(0.1,Vector3f::UnitY()) * Translation3f(-vector_moon);
    M_moon=affine_moon.matrix()*M_moon;

    //Shader de moon
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M_moon.data());
    glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
    glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

    glUniform3f(_shader.getUniformLocation("light_pos"), 5.0, 1.0, 5.0);

    normal= ((_cam.viewMatrix().topLeftCorner(3,3) * M_moon.topLeftCorner(3,3)).inverse()).transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, normal.data());
    _mesh.draw(_shader);

  if(_wireframe)
  {
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDepthFunc(GL_LEQUAL);

    _mesh.draw(_shader);
    
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthFunc(GL_LESS);
  }

  _shader.deactivate();
}

void Viewer::drawScene3D()
{
  
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();
  glUniform1i(_shader.getUniformLocation("wireframe"), 0);

  Affine3f A;
  A.setIdentity();
  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"),1,GL_FALSE,A.matrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("view_mat"),1,GL_FALSE,_cam.viewMatrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"),1,GL_FALSE,_cam.projectionMatrix().data());

  //Valeur de ce vecteur fixé aléatoirement 
  glUniform3f(_shader.getUniformLocation("light_pos"), 5.0, 1.0, 5.0);

  Eigen::Matrix3f normal;
  normal= ((_cam.viewMatrix().topLeftCorner(3,3) * A.matrix().topLeftCorner(3,3)).inverse()).transpose();
  glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, normal.data());
  _mesh.draw(_shader);


  if(_wireframe)
  {
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDepthFunc(GL_LEQUAL);

    _mesh.draw(_shader);
    
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthFunc(GL_LESS);
  }

  _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
    // drawScene();
    // drawScene2D();
    // drawScene3DSolarSystem();
    drawScene3D();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    // _shader.loadFromFiles(DATA_DIR"/shaders/simple.vert", DATA_DIR"/shaders/simple.frag");
    _shader.loadFromFiles(DATA_DIR"/shaders/blinn.vert", DATA_DIR"/shaders/blinn.frag");
    checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
  if(key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    loadShaders();
  }

  if(action == GLFW_PRESS || action == GLFW_REPEAT )
  {
    if (key==GLFW_KEY_UP)
    {
      _offset.y() += 0.05f;
    }
    else if (key==GLFW_KEY_DOWN)
    {
      _offset.y() -= 0.05f;
    }
    else if (key==GLFW_KEY_LEFT)
    {
      _offset.x() -= 0.05f;
    }
    else if (key==GLFW_KEY_RIGHT)
    {
      _offset.x() += 0.05f;
    }
    else if (key==GLFW_KEY_PAGE_UP)
    {
      _zoom *= 1.1f;
    }
    else if (key==GLFW_KEY_PAGE_DOWN)
    {
      _zoom /= 1.1f;
    }
    else if (key==GLFW_KEY_W)//clavier qwerty
    {
      _wireframe = 1;
    }
    else if (key==GLFW_KEY_X)
    {
      _wireframe = 0;
    }
    else if (key==GLFW_KEY_H)
    {
      _theta += 0.1;
      _vecAxis = Vector3f::UnitX();
      _tAxis = {0.5, 0, 0};
    }
    else if (key==GLFW_KEY_N)
    {
      _theta -= 0.1;
      _vecAxis = Vector3f::UnitX();
      _tAxis = {0.5, 0, 0};
    }
        else if (key==GLFW_KEY_T)
    {
      _theta += 0.1;
      _vecAxis = Vector3f::UnitY();
      _tAxis = {0, 0.5, 0};
    }
        else if (key==GLFW_KEY_Y)
    {
      _theta -= 0.1;
      _vecAxis = Vector3f::UnitY();
      _tAxis = {0, 0.5, 0};
    }
  }
}

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow */*window*/, int /*button*/, int action)
{
  if(action == GLFW_PRESS)
  {
      _trackingMode = TM_ROTATE_AROUND;
      _trackball.start();
      _trackball.track(_lastMousePos);
  }
  else if(action == GLFW_RELEASE)
  {
      _trackingMode = TM_NO_TRACK;
  }
}


/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
    if(_trackingMode == TM_ROTATE_AROUND)
    {
        _trackball.track(Vector2i(x,y));
    }

    _lastMousePos = Vector2i(x,y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
  _cam.zoom(-0.1f*y);
}

void Viewer::charPressed(int /*key*/)
{
}
