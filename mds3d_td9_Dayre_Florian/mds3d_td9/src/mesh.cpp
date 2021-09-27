#include "mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "shader.h"
#include <Eigen/Geometry>
#include <limits>

using namespace std;
using namespace Eigen;
using namespace surface_mesh;


void Mesh::subdivide()
{
  // TODO !
  //Etape 3
  surface_mesh::Surface_mesh new_mesh;
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> vertex_mapping = _halfedge.add_vertex_property<Surface_mesh::Vertex>("v:mapping");
  int di;
  float B;

  for(Surface_mesh::Vertex_iterator vit = _halfedge.vertices_begin();
      vit != _halfedge.vertices_end(); ++vit)
  {
    Surface_mesh::Vertex vi = *vit;
    Surface_mesh::Halfedge hi = _halfedge.halfedge(vi);
    Surface_mesh::Halfedge h = hi;
    Point sum = Point(0,0,0);
    di = 0;

    Point pi;
    if(_halfedge.is_boundary(vi))
    {
      do
      {
        if(_halfedge.is_boundary(_halfedge.edge(h)))
          sum += _halfedge.position(_halfedge.to_vertex(h));
        h = _halfedge.next_halfedge(_halfedge.opposite_halfedge(h));
      }while(h != hi);

      pi = 3./4.*_halfedge.position(vi) + 1./8.*sum;
    }
    else
    {
      do
      {
        sum += _halfedge.position(_halfedge.to_vertex(h));
        di++;
        h = _halfedge.next_halfedge(_halfedge.opposite_halfedge(h));
      }while(h != hi);

      if(di == 3)
        B = 3./16.;
      else if (di > 3)
        B = 1./di*(5./8. - (3./8+((1./4)*cos((2*M_PI)/di)))*(3./8+((1./4)*cos((2*M_PI)/di))));
      
      pi = (1-B*di)*_halfedge.position(vi) + B*sum;
    }
   
    vertex_mapping[vi] = new_mesh.add_vertex(pi);
  }
/*
  for(Surface_mesh::Face_iterator fit = _halfedge.faces_begin();
      fit != _halfedge.faces_end(); ++fit)
  {
    Surface_mesh::Face fi = *fit;

    Surface_mesh::Halfedge h1 = _halfedge.halfedge(fi);
    Surface_mesh::Vertex v1 = _halfedge.to_vertex(h1);

    Surface_mesh::Halfedge h2 = _halfedge.next_halfedge(h1);
    Surface_mesh::Vertex v2 = _halfedge.to_vertex(h2);

    Surface_mesh::Halfedge h3 = _halfedge.next_halfedge(h2);
    Surface_mesh::Vertex v3 = _halfedge.to_vertex(h3);

    new_mesh.add_triangle(vertex_mapping[v1], vertex_mapping[v2], vertex_mapping[v3]);
  }
*/
  //Etape 1
  Surface_mesh::Edge_property<Surface_mesh::Vertex> edge_mapping = _halfedge.add_edge_property<Surface_mesh::Vertex>("e:mapping");
  for(Surface_mesh::Edge_iterator eit = _halfedge.edges_begin();
      eit != _halfedge.edges_end(); ++eit)
  {
    Surface_mesh::Edge ei = *eit;

    Surface_mesh::Halfedge he = _halfedge.halfedge(ei,0);

    Surface_mesh::Vertex v0 = _halfedge.to_vertex(he);
    Surface_mesh::Vertex v2 = _halfedge.to_vertex(_halfedge.opposite_halfedge(he));

    Surface_mesh::Vertex v1 = _halfedge.to_vertex(_halfedge.next_halfedge(he));

    Surface_mesh::Halfedge h1 = _halfedge.opposite_halfedge(he);
    Surface_mesh::Vertex v3 = _halfedge.to_vertex(_halfedge.next_halfedge(h1));

    Point u;
    if(_halfedge.is_boundary(ei))
      u = 0.5*(_halfedge.position(v0)+_halfedge.position(v2));
    else
      u = 3./8.*_halfedge.position(v0) + 3./8.*_halfedge.position(v2) + 1./8.*_halfedge.position(v1) + 1./8.*_halfedge.position(v3);

    edge_mapping[ei] = new_mesh.add_vertex(u);
  }

  //Etape 2
  for(Surface_mesh::Face_iterator fit = _halfedge.faces_begin();
      fit != _halfedge.faces_end(); ++fit)
  {
    Surface_mesh::Face fi = *fit;

    Surface_mesh::Halfedge h1 = _halfedge.halfedge(fi);
    Surface_mesh::Vertex v0 = vertex_mapping[_halfedge.to_vertex(h1)];
    Surface_mesh::Edge e1 = _halfedge.edge(h1);
    Surface_mesh::Vertex u1 = edge_mapping[e1];


    Surface_mesh::Halfedge h2 = _halfedge.next_halfedge(h1);
    Surface_mesh::Vertex v3 = vertex_mapping[_halfedge.to_vertex(h2)];
    Surface_mesh::Edge e2 = _halfedge.edge(h2);
    Surface_mesh::Vertex u4 = edge_mapping[e2];

    Surface_mesh::Halfedge h3 = _halfedge.next_halfedge(h2);
    Surface_mesh::Vertex v2 = vertex_mapping[_halfedge.to_vertex(h3)];
    Surface_mesh::Edge e3 = _halfedge.edge(h3);
    Surface_mesh::Vertex u2 = edge_mapping[e3];

    new_mesh.add_triangle(v0, u4, u1);
    new_mesh.add_triangle(u4, v3, u2);
    new_mesh.add_triangle(u2, v2, u1);
    new_mesh.add_triangle(u1, u4, u2);
  }

  new_mesh.update_face_normals();
  new_mesh.update_vertex_normals();
  
  _halfedge = new_mesh;
  updateHalfedgeToMesh();
  updateVBO();
}

Mesh::~Mesh()
{
  if(_isInitialized)
  {
    glDeleteBuffers(1,&_vertexBufferId);
    glDeleteBuffers(1,&_indexBufferId);
    glDeleteVertexArrays(1,&_vertexArrayId);
  }
}


bool Mesh::load(const std::string& filename)
{
    std::cout << "Loading: " << filename << std::endl;

    if(!_halfedge.read(filename))
      return false;

    _halfedge.update_face_normals();
    _halfedge.update_vertex_normals();

    updateHalfedgeToMesh();

    return true;
}

void Mesh::updateHalfedgeToMesh()
{
    // vertex properties
    Surface_mesh::Vertex_property<Point> vertices = _halfedge.get_vertex_property<Point>("v:point");
    Surface_mesh::Vertex_property<Point> vnormals = _halfedge.get_vertex_property<Point>("v:normal");
    Surface_mesh::Vertex_property<Texture_coordinate> texcoords = _halfedge.get_vertex_property<Texture_coordinate>("v:texcoord");
    Surface_mesh::Vertex_property<Color> colors = _halfedge.get_vertex_property<Color>("v:color");

    // vertex iterator
    Surface_mesh::Vertex_iterator vit;

    Vector3f pos;
    Vector3f normal;
    Vector2f tex;
    Vector4f color;
    _vertices.clear();
    for(vit = _halfedge.vertices_begin(); vit != _halfedge.vertices_end(); ++vit)
    {
        pos = vertices[*vit];
        normal = vnormals[*vit];
        if(texcoords)
            tex = texcoords[*vit];
        if(colors)
            color << colors[*vit], 1.0f;
        else
            color = Vector4f(0.6f,0.6f,0.6f,1.0f);

        _vertices.push_back(Vertex(pos,normal,color,tex));
    }

    // face iterator
    Surface_mesh::Face_iterator fit, fend = _halfedge.faces_end();
    // vertex circulator
    Surface_mesh::Vertex_around_face_circulator fvit, fvend;
    Surface_mesh::Vertex v0, v1, v2;
    _faces.clear();
    for (fit = _halfedge.faces_begin(); fit != fend; ++fit)
    {
        fvit = fvend = _halfedge.vertices(*fit);
        v0 = *fvit;
        ++fvit;
        v2 = *fvit;

        do{
            v1 = v2;
            ++fvit;
            v2 = *fvit;
            _faces.push_back(Vector3i(v0.idx(),v1.idx(),v2.idx()));
        } while (++fvit != fvend);
    }

    //updateNormals();
}

void Mesh::init()
{
    glGenVertexArrays(1,&_vertexArrayId);
    glGenBuffers(1,&_vertexBufferId);
    glGenBuffers(1,&_indexBufferId);

    updateVBO();

    _isInitialized = true;
}

void Mesh::updateNormals()
{
    // pass 1: set the normal to 0
    for(std::vector<Vertex>::iterator v_iter = _vertices.begin() ; v_iter!=_vertices.end() ; ++v_iter)
        v_iter->normal.setZero();

    // pass 2: compute face normals and accumulate
    for(std::size_t j=0; j<_faces.size(); ++j)
    {
        Vector3f v0 = _vertices[_faces[j][0]].position;
        Vector3f v1 = _vertices[_faces[j][1]].position;
        Vector3f v2 = _vertices[_faces[j][2]].position;

        Vector3f n = (v1-v0).cross(v2-v0).normalized();

        _vertices[_faces[j][0]].normal += n;
        _vertices[_faces[j][1]].normal += n;
        _vertices[_faces[j][2]].normal += n;
    }

    // pass 3: normalize
    for(std::vector<Vertex>::iterator v_iter = _vertices.begin() ; v_iter!=_vertices.end() ; ++v_iter)
        v_iter->normal.normalize();
}

void Mesh::updateVBO()
{
  glBindVertexArray(_vertexArrayId);

  // activate the VBO:
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
  // copy the data from host's RAM to GPU's video memory:
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertices.size(), _vertices[0].position.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i)*_faces.size(), _faces[0].data(), GL_STATIC_DRAW);


}


void Mesh::draw(const Shader& shd)
{
    if (!_isInitialized)
      init();

      // Activate the VBO of the current mesh:
  glBindVertexArray(_vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

  // Specify vertex data

  // 1 - get id of the attribute "vtx_position" as declared as "in vec3 vtx_position" in the vertex shader
  int vertex_loc = shd.getAttribLocation("vtx_position");
  if(vertex_loc>=0)
  {
    // 2 - tells OpenGL where to find the x, y, and z coefficients:
    glVertexAttribPointer(vertex_loc,     // id of the attribute
                          3,              // number of coefficients (here 3 for x, y, z)
                          GL_FLOAT,       // type of the coefficients (here float)
                          GL_FALSE,       // for fixed-point number types only
                          sizeof(Vertex), // number of bytes between the x coefficient of two vertices
                                          // (e.g. number of bytes between x_0 and x_1)
                          0);             // number of bytes to get x_0
    // 3 - activate this stream of vertex attribute
    glEnableVertexAttribArray(vertex_loc);
  }


  int normal_loc = shd.getAttribLocation("vtx_normal");
  if(normal_loc>=0)
  {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
    glEnableVertexAttribArray(normal_loc);
  }

  int color_loc = shd.getAttribLocation("vtx_color");
  if(color_loc>=0)
  {
    glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector3f)));
    glEnableVertexAttribArray(color_loc);
  }

  int texcoord_loc = shd.getAttribLocation("vtx_texcoord");
  if(texcoord_loc>=0)
  {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector3f)+sizeof(Vector4f)));
    glEnableVertexAttribArray(texcoord_loc);
  }

  // send the geometry
  glDrawElements(GL_TRIANGLES, (gl::GLsizei)(3*_faces.size()), GL_UNSIGNED_INT, 0);

  // at this point the mesh has been drawn and raserized into the framebuffer!
  if(vertex_loc>=0)   glDisableVertexAttribArray(vertex_loc);
  if(normal_loc>=0)   glDisableVertexAttribArray(normal_loc);
  if(color_loc>=0)    glDisableVertexAttribArray(color_loc);
  if(texcoord_loc>=0) glDisableVertexAttribArray(texcoord_loc);

  checkError();
}

