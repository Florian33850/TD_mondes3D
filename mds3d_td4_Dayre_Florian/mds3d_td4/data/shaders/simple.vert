#version 330 core

in vec3 vtx_position;
in vec3 vtx_color;
out vec3 var_color;

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;

void main()
{
  gl_Position = proj_mat * view_mat * obj_mat * vec4(vtx_position, 1.);
  
  var_color = vtx_color;
}
