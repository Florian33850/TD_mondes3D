#version 330 core

#define M_PI 3.14159265358979323846

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;

in vec3 vtx_position; //cylinder
in vec3 vtx_normal;
in vec2 vtx_texcoord; //uv

out vec3 v_normal;
out vec2 v_uv;

vec3 cylinder(vec2 uv, vec3 A, vec3 B, float r)
{
  //return vec3(5,5,5) + r*vec3(cos(uv.y)*cos(uv.x), cos(uv.y)*sin(uv.x), sin(uv.y)); //capture 1
  //return vec3(uv.x, uv.y*cos(2*3.1415926535897932384626433832795*uv.y), uv.y*sin(2*3.1415926535897932384626433832795*uv.y));//capture 2
  //return vec3(uv.x, 10*cos(2*3.1415926535897932384626433832795*uv.y), 10*sin(2*3.1415926535897932384626433832795*uv.y));//capture 3
  //return vec3(uv.x, 1*cos(2*3.1415926535897932384626433832795*uv.y), 1*sin(2*3.1415926535897932384626433832795*uv.y));//capture 4
  //return r*vec3(uv.x, 0.5*cos(2*3.1415926535897932384626433832795*uv.y), 0.5*sin(2*3.1415926535897932384626433832795*uv.y));//capture 5

  vec3 t = B-A;
  vec3 tmp = vec3(4,6,7);
  vec3 n = cross(t,tmp);
  vec3 b = cross(t,n);
  normalize(b);
  normalize(n);
  //return r*vec3(uv.x*t.x, n.y*cos(2*3.1415926535897932384626433832795*uv.y), b.x*sin(2*3.1415926535897932384626433832795*uv.y));//capture 6

  float angle = 2 * M_PI * uv.y;
  float x = r * cos(angle);
  float y = r * sin(angle);

  return A + x * b + y * n + uv.x * t;
}

vec3 bezier(float u, vec3 B[4], out vec3 T)
{
  vec3 T = normalize(B[1] + B[2]*2*u + B[3]*3*u*u);

  return B[0] + B[1]*u + B[2]*(u*u) + B[3]*(u*u*u);
}

vec3 cylBezierYZ(vec2 uv, vec3 B[4], float r)
{
  vec3 T;
  vec3 q = bezier(uv.y, B, T);
  
  float angle = 2 * M_PI * uv.x;
  float x = r * cos(angle);
  float y = r * sin(angle);

  return vec3(x, y, 0) + q;
}

void main()
{
  /* cylinder */
  vec3 a = vec3(0,1,5);
  vec3 b = vec3(2,2,0);
  float r = 0.5;

  /* Bezier */
  vec3 B[4];
  B[0] = vec3(-1,0,2);
  B[1] = vec3(-0.3,0,4);
  B[2] = vec3(0.3,0,1);
  B[3] = vec3(1,0,-0.5);
  
  v_uv  = vtx_texcoord;
  v_normal = normalize(normal_mat * vtx_normal);
  //Cylindre
  //vec4 p = view_mat * (obj_mat * vec4(cylinder(vtx_texcoord, a, b, r), 1.));
  //Bezier
  vec4 p = view_mat * (obj_mat * vec4(cylBezierYZ(vtx_texcoord, B, r), 1.));
  gl_Position = proj_mat * p;
}
