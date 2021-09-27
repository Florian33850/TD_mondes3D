#include "viewer.h"
#include "camera.h"
#include "SOIL2.h"

using namespace Eigen;

Viewer::Viewer()
  : _winWidth(0), _winHeight(0), _theta(0), _rotate(true)
{
}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h){

    // Background color
    glClearColor(0.0, 0.0, 0.0, 0.0);

    loadShaders();
    // if(!_mesh.load(DATA_DIR"/models/earth.obj")) exit(1);
    if(!_mesh.load(DATA_DIR"/models/earth2.obj")) exit(1);

    _mesh.initVBA();

    reshape(w,h);
    _cam.setPerspective(M_PI/3,0.1f,20000.0f);
    _cam.lookAt(Vector3f(0,0,4), Vector3f(0,0,0), Vector3f(0,1,0));
    _trackball.setCamera(&_cam);

 // \param filename the name of the file to upload as a texture
	  // \param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	  // \param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	  // \param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	  // \return 0-failed, otherwise returns the OpenGL texture handle
    _texture_1 = SOIL_load_OGL_texture(DATA_DIR"/textures/earth.jpg", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_1 == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_2 = SOIL_load_OGL_texture(DATA_DIR"/textures/earth_clouds.jpg", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_2 == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_3 = SOIL_load_OGL_texture(DATA_DIR"/textures/earth_night.jpg", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_3 == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

     _texture_4 = SOIL_load_OGL_texture(DATA_DIR"/textures/checkerboard.png", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_4 == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_5= SOIL_load_OGL_texture(DATA_DIR"/textures/cow.jpg", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_5 == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_normals= SOIL_load_OGL_texture(DATA_DIR"/textures/normals.png", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_normals == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_diffuse= SOIL_load_OGL_texture(DATA_DIR"/textures/diffuse.png", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_diffuse == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    _texture_earth_normal= SOIL_load_OGL_texture(DATA_DIR"/textures/earth_normal.jpg", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( _texture_earth_normal == 0 ) std::cerr << "Soil loading error : " << SOIL_last_result() << std::endl;

    glGenSamplers(1, &_samplerId);

    glEnable(GL_DEPTH_TEST);
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
    // configure the rendering target size (viewport)
    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader.activate();
    glUniformMatrix4fv(_shader.getUniformLocation("view_mat"),1,GL_FALSE,_cam.viewMatrix().data());
    glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"),1,GL_FALSE,_cam.projectionMatrix().data());

    Affine3f M(AngleAxisf(_theta,Vector3f(0,1,0)));
    Affine3f A;
    A.setIdentity();

    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"),1,GL_FALSE,A.matrix().data());

    Matrix4f matLocal2Cam = _cam.viewMatrix() * A.matrix();
    Matrix3f matN = matLocal2Cam.topLeftCorner<3,3>().inverse().transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"),1,GL_FALSE,matN.data());

    Vector3f lightDir = Vector3f(1,0,1).normalized();
    lightDir = (matLocal2Cam.topLeftCorner<3,3>() * lightDir).normalized();
    // lightDir = (_cam.viewMatrix().topLeftCorner<3,3>() * lightDir).normalized();
    glUniform3fv(_shader.getUniformLocation("lightDir"),1,lightDir.data());

    //Earth
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_1);
    glUniform1i(_shader.getUniformLocation("text2D_1"),0);
    glBindSampler(0, _samplerId);

    //Earth clouds
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture_2);
    glUniform1i(_shader.getUniformLocation("text2D_2"),1);
    glBindSampler(1, _samplerId);

    //Earth night
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture_3);
    glUniform1i(_shader.getUniformLocation("text2D_3"),2);
    glBindSampler(2, _samplerId);

    //plane
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _texture_4);
    glUniform1i(_shader.getUniformLocation("text2D_4"),3);
    glBindSampler(3, _samplerId);

    //Cow
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _texture_5);
    glUniform1i(_shader.getUniformLocation("text2D_5"),4);
    glBindSampler(4, _samplerId);

    //Mipmap
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, _texture_mipmap);
    glUniform1i(_shader.getUniformLocation("text2D_mipmap"),5);
    glBindSampler(5, _samplerId);

    //Normals
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, _texture_normals);
    glUniform1i(_shader.getUniformLocation("text2D_normals"),6);
    glBindSampler(6, _samplerId);

    //Diffuse
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, _texture_diffuse);
    glUniform1i(_shader.getUniformLocation("text2D_diffuse"),7);
    glBindSampler(7, _samplerId);

    //Earth normal
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, _texture_earth_normal);
    glUniform1i(_shader.getUniformLocation("text2D_earth_normal"),8);
    glBindSampler(8, _samplerId);
    
    _mesh.draw(_shader);

    _shader.deactivate();
}


void Viewer::updateAndDrawScene()
{
  if(_rotate)
    _theta += 0.01*M_PI;
  drawScene();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    // _shader.loadFromFiles(DATA_DIR"/shaders/simple.vert", DATA_DIR"/shaders/simple.frag");
    // _shader.loadFromFiles(DATA_DIR"/shaders/intro.vert", DATA_DIR"/shaders/intro.frag");
    _shader.loadFromFiles(DATA_DIR"/shaders/normalMapping.vert", DATA_DIR"/shaders/normalMapping.frag");


    checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

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
  _cam.zoom(-0.1*y);
}

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
  else if(key == GLFW_KEY_A && action == GLFW_PRESS)
  {
    _rotate = !_rotate;
  }

  if(action == GLFW_PRESS || action == GLFW_REPEAT )
  {
    if (key==GLFW_KEY_UP)
    {
    }
    else if (key==GLFW_KEY_DOWN)
    {
    }
    else if (key==GLFW_KEY_LEFT)
    {
    }
    else if (key==GLFW_KEY_RIGHT)
    {
    }
    else if (key==GLFW_KEY_PAGE_UP)
    {
    }
    else if (key==GLFW_KEY_PAGE_DOWN)
    {
    }
  }
}

void Viewer::charPressed(int /*key*/)
{
}
