#version 330 core

in vec3 vtx_position;
in vec3 vtx_color;
in vec3 vtx_normal;
out vec3 var_color;

uniform mat4 obj_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform mat3 normal_mat;
uniform vec3 light_pos;

out vec3 var_light_dir;
out vec3 var_view_pos;
out vec3 var_normal;

void main()
{

  gl_Position = proj_mat * view_mat * obj_mat * vec4(vtx_position.xyz,1.);

  vec4 view_pos = view_mat * obj_mat * vec4(vtx_position,1);
  vec3 normal = normalize(normal_mat * vtx_normal);
  vec3 light_dir = light_pos - view_pos.xyz;
  vec4 light_mat = view_mat * vec4(light_dir,1);
  light_dir = normalize(light_mat.xyz);

  var_view_pos = view_pos.xyz;
  var_normal = normal;
  var_light_dir = light_dir;
  var_color = vtx_color;
}
