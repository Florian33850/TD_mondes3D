#version 330 core

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;

in vec3 vtx_position;
in vec2 vtx_text_coord;

out vec2 v_text_coord;

void main()
{
  vec4 p = view_mat * (obj_mat * vec4(vtx_position, 1.));
  v_text_coord = vtx_text_coord;
  gl_Position = proj_mat * p;
}
